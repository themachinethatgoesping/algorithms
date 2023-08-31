#!/bin/env python3

import os
import shutil
import sys
import subprocess
from tqdm import tqdm
from copy import deepcopy
import hashlib

# get all header files
headers = []
for r, d, f in os.walk('.'):
    for file in f:
        if file.endswith(".doc.hpp"):
            continue
        if file.endswith("pch.hpp"):
            continue

        if file.endswith('.hpp'):
            headers.append(r + '/' + file)
        if file.endswith('.cpp'):
            headers.append(r + '/' + file)
headers.sort()

includes = set()

output_path = "pch/pch.hpp"
        
os.makedirs("/".join(output_path.split('/')[:-1]), exist_ok=True)
        
with open('make_headers.log', 'w', encoding="utf-8") as ofi_log:
    prg = tqdm(headers)
    for header in prg:
        if len(header) > 53:
            prg.set_postfix_str(f"...{header[-50:]}")
        else:
            prg.set_postfix_str(header)

        filename = header.split("/")[-1]
        
        with open (header, 'r', encoding="utf-8") as ifi:
            for line in ifi:
                if "#include" in line:
                    if not "themachinethatgoesping" in line:
                        if "<" in line:
                            if ">" in line:
                                includes.add(line.split(">")[0] + ">")

with open(output_path, 'w', encoding="utf-8") as ofi:
    #ofi.write("#define PCH\n")
    
    for include in sorted(includes):
        ofi.write(include + "\n")
        
    #ofi.write("#undef PCH\n")
    ofi.write("\n")