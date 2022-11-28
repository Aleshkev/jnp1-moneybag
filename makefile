
test:
	echo "Running tests. Tests 604-616 should NOT compile."
	for test in \
		  	101 102 103 104 105 106 \
		  	201 202 203 \
		  	301 302 303 \
		  	401 402 \
		  	501 502 \
			601 602 603 \
			604 605 606 607 608 609 610 611 612 613 614 615 616 ; do \
  		echo $$test ;\
  		g++-10 -Wall -Wextra -O2 -std=c++20 moneybag_test_external.cc moneybag_test.cc moneybag.cc -DTEST_NUM=$$test ;\
		./a.out ;\
	done
