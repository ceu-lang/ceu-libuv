CEU_DIR = $(error set absolute path to "<ceu>" repository)

samples:
	for i in samples/async-*.ceu \
	         samples/timer-*.ceu \
	         samples/threads-*.ceu \
	         samples/fs-*.ceu \
	         samples/tcp-*.ceu; \
	do                         \
		echo;                                                               \
		echo "#########################################################";   \
		echo File: "$$i -> /tmp/$$(basename $$i .ceu)";	                    \
		echo "#########################################################";   \
		echo -n "Press <enter> to start...";                                \
		read _;                                                             \
	    echo ceu --pre --pre-args=\"-I$(CEU_DIR)/include -I./include -I./samples\" \
	              --pre-input=$$i                                           \
	        --ceu --ceu-err-unused=pass --ceu-err-uninitialized=pass        \
	        --env --env-types=$(CEU_DIR)/env/types.h                        \
	              --env-threads=./env/threads.h                             \
	              --env-main=$(CEU_DIR)/env/main.c                          \
	        --cc --cc-args=\"-lm -llua5.3 -luv\"                            \
	             --cc-output=/tmp/$$(basename $$i .ceu);                    \
	    ceu --pre --pre-args="-I$(CEU_DIR)/include -I./include -I./samples/" \
	              --pre-input=$$i                                           \
	        --ceu --ceu-err-unused=pass --ceu-err-uninitialized=pass        \
	        --env --env-types=$(CEU_DIR)/env/types.h                        \
	              --env-threads=./env/threads.h                             \
	              --env-main=$(CEU_DIR)/env/main.c                          \
	        --cc --cc-args="-lm -llua5.3 -luv"                              \
	             --cc-output=/tmp/$$(basename $$i .ceu);                    \
		cd samples && /tmp/$$(basename $$i .ceu) && cd ..;	                \
		echo ">>> OK";                                                      \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
	done

.PHONY: samples
