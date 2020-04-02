#!/bin/bash
if diff -w -y --left-column ../data/result.txt ../projects/student/result.txt; then   
	printf "============\n"    
    printf "AC\n"    
else  
	printf "============\n"    
    printf "WA\n"   
    exit 0  
fi  
