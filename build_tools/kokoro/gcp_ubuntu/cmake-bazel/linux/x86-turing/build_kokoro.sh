#!/bin/bash

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

# Build and test the python bindings and tensorflow integrations on GPU within
# gcr.io/iree-oss/cmake-bazel-tensorflow-nvidia
# Requires the environment variables KOKORO_ROOT and KOKORO_ARTIFACTS_DIR, which
# are set by Kokoro.

set -x
set -e
set -o pipefail

# Print the UTC time when set -x is on
export PS4='[$(date -u "+%T %Z")] '

source "${KOKORO_ARTIFACTS_DIR?}/github/iree/build_tools/kokoro/gcp_ubuntu/docker_common.sh"

# Sets DOCKER_RUN_ARGS
docker_setup

docker run "${DOCKER_RUN_ARGS[@]?}" \
  gcr.io/iree-oss/cmake-bazel-tensorflow-nvidia@sha256:8c2b5247f202dbd32051a78cec0228ea102d38075fd212c2ce01d498fbd5d322 \
  build_tools/kokoro/gcp_ubuntu/cmake-bazel/linux/x86-turing/build.sh

# Kokoro will rsync this entire directory back to the executor orchestrating the
# build which takes forever and is totally useless.
rm -rf "${KOKORO_ARTIFACTS_DIR?}"/*

# Print out artifacts dir contents after deleting them as a coherence check.
ls -1a "${KOKORO_ARTIFACTS_DIR?}/"
