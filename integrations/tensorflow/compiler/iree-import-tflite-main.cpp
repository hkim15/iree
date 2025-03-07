// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/ToolOutputFile.h"
#include "mlir/Dialect/Quant/QuantOps.h"
#include "mlir/Dialect/StandardOps/IR/Ops.h"
#include "mlir/Dialect/Tosa/IR/TosaOps.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OperationSupport.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/FileUtilities.h"
#include "tensorflow/compiler/mlir/lite/flatbuffer_import.h"
#include "tensorflow/compiler/mlir/lite/ir/tfl_ops.h"
#include "tensorflow/compiler/mlir/tensorflow/ir/tf_ops.h"
#include "tensorflow/compiler/mlir/tosa/tfl_passes.h"

using namespace llvm;
using namespace mlir;

int main(int argc, char **argv) {
  llvm::InitLLVM y(argc, argv);

  static cl::opt<std::string> inputPath(
      cl::Positional, cl::desc("<TFLite FlatBuffer>"), cl::Required);
  static cl::opt<std::string> outputFilename("o", cl::desc("Output filename"),
                                             cl::value_desc("filename"),
                                             cl::init("-"));
  static llvm::cl::opt<std::string> saveTempTflInput(
      "save-temp-tfl-input",
      llvm::cl::desc("Save the TFL pipeline input to this file"),
      llvm::cl::init(""));
  static llvm::cl::opt<std::string> saveTempIreeImport(
      "save-temp-iree-input",
      llvm::cl::desc("Save the resultant IR to this file (useful for saving an "
                     "intermediate in a pipeline)"),
      llvm::cl::init(""));

  static cl::list<std::string> inputArrayFlag(
      "input-array",
      llvm::cl::desc("Input tensor, if different from the default inputs"),
      llvm::cl::ZeroOrMore);
  static cl::list<std::string> outputArrayFlag(
      "output-array",
      llvm::cl::desc("Output tensor, if different from the default outputs"),
      llvm::cl::ZeroOrMore);

  // Register any command line options.
  registerAsmPrinterCLOptions();
  registerMLIRContextCLOptions();
  cl::ParseCommandLineOptions(argc, argv);

  // Initialize dialects.
  DialectRegistry registry;
  registry.insert<mlir::TFL::TensorFlowLiteDialect>();
  registry.insert<mlir::tosa::TosaDialect>();
  registry.insert<quant::QuantizationDialect>();
  registry.insert<TF::TensorFlowDialect>();
  registry.insert<StandardOpsDialect>();

  // Convert the Module proto into MLIR.
  MLIRContext context;
  registry.loadAll(&context);

  // Load input buffer.
  std::string errorMessage;
  auto inputFile = openInputFile(inputPath, &errorMessage);
  if (!inputFile) {
    llvm::errs() << errorMessage << "\n";
    return 1;
  }

  // Convert.
  std::vector<std::string> inputArrays(inputArrayFlag.begin(),
                                       inputArrayFlag.end());
  std::vector<std::string> outputArrays(outputArrayFlag.begin(),
                                        outputArrayFlag.end());
  auto loc = mlir::FileLineColLoc::get(inputFile->getBufferIdentifier(), 0, 0,
                                       &context);
  OwningModuleRef module = tflite::FlatBufferToMlir(
      absl::string_view(inputFile->getBufferStart(),
                        inputFile->getBufferSize()),
      &context, loc,
      /*use_external_constant=*/false, inputArrays, outputArrays);
  if (!module) {
    // Error should have emitted.
    llvm::errs() << "Unable to import TFLite flatbuffer to MLIR Module\n";
    return 2;
  }

  // Save.
  auto saveToFile = [&](llvm::StringRef savePath) -> LogicalResult {
    auto outputFile = openOutputFile(savePath);
    if (!outputFile) {
      llvm::errs() << "Could not open output file: " << savePath << "\n";
      return failure();
    }
    OpPrintingFlags printFlags;
    printFlags.enableDebugInfo();
    module->print(outputFile->os(), printFlags);
    outputFile->os() << "\n";
    outputFile->keep();
    return success();
  };

  // Save temp input.
  if (!saveTempTflInput.empty()) {
    if (failed(saveToFile(saveTempTflInput))) return 10;
  }

  // Run transformations.
  mlir::tosa::TOSATFLLegalizationPipelineOptions tosaOptions;
  PassManager pm(&context, PassManager::Nesting::Implicit);
  applyPassManagerCLOptions(pm);
  mlir::tosa::createTFLtoTOSALegalizationPipeline(pm, tosaOptions);
  if (failed(pm.run(*module))) {
    llvm::errs() << "Running iree-import-tflite pass pipeline failed (see "
                    "diagnostics)\n";
    return 3;
  }

  // Save temp output.
  if (!saveTempIreeImport.empty()) {
    if (failed(saveToFile(saveTempIreeImport))) return 10;
  }

  // Save output.
  if (failed(saveToFile(outputFilename))) return 3;
  return 0;
}
