#!/bin/bash
for i in Originals/*; do ./fixfile $i > uni/`basename "${i%.bas}.uni"` ; done