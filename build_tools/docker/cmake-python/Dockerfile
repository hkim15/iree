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

# An image for building IREE and its Python bindings using CMake.

FROM gcr.io/iree-oss/cmake@sha256:9d9953acf5ca0cf1ff3e8de32f10f24dfab1c4e8ec5d1fc047f556024ee4bed6 AS final
# Dependencies for the python bindings tests.
RUN apt-get update \
  && apt-get install -y \
    python3 \
    python3-pip \
    python3-setuptools \
  && python3 -m pip install --upgrade pip \
  && python3 -m pip install numpy==1.19.4 absl-py

ENV PYTHON_BIN /usr/bin/python3
