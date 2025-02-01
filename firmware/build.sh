#!/bin/bash
cmake jorca --preset=Release
pushd jorca/build/Release && ninja && popd