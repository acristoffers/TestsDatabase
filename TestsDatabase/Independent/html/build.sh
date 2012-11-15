#!/usr/bin/env bash

ruby blob_generator.rb
./compressor blob.txt > blob
rm blob.txt compressor blob_generator.rb $0