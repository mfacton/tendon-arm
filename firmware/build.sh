#!/bin/bash
cmake jorcaf4 --preset=Release
pushd jorcaf4/build/Release && ninja && popd