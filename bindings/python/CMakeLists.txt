# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(NUMPY_DEPS "")

set(PYBIND_COPTS "-fexceptions")
set(PYBIND_EXTENSION_COPTS "-fvisibility=hidden")

# Generated setup scripts.
# TODO: Make the version configurable.
set(IREE_PYTHON_VERSION "0.1a1")
configure_file(setup.py setup.py COPYONLY)
configure_file(setup_compiler.py.in setup_compiler.py)
configure_file(setup_runtime.py.in setup_runtime.py)
configure_file(setup_tools_core.py.in setup_tools_core.py)
configure_file(setup_tools_tf.py.in setup_tools_tf.py)

# Namespace packages.
add_subdirectory(pyiree/compiler2)
add_subdirectory(pyiree/rt)

if(${IREE_BUILD_COMPILER})
add_subdirectory(pyiree/tools/core)
endif()

# Tests.
add_subdirectory(tests)
