//
// Created by Shuhao Zhang on 17/10/19.
//

#ifndef ALLIANCEDB_T_TIMER_H
#define ALLIANCEDB_T_TIMER_H


#include "rdtsc.h"              /* startTimer, stopTimer */
#include <sys/time.h>           /* gettimeofday */
#include <stdio.h>              /* printf */
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "../utils/types.h"
#include <chrono>

using namespace std::chrono;

//thread_local structure.

struct T_TIMER {
#ifndef NO_TIMING
    struct timeval start, end;
    uint64_t overall_timer, partition_timer_pre, partition_timer;
    uint64_t wait_timer_pre = 0, wait_timer = 0;
    uint64_t buildtimer_pre = 0, buildtimer = 0;
    uint64_t debuildtimer_pre = 0, debuildtimer = 0;//buildtimer is accumulated.
    uint64_t sorttimer_pre = 0, sorttimer = 0;//accumulate.
    uint64_t mergetimer_pre = 0, mergetimer = 0;//accumulate.
    uint64_t join_partitiontimer_pre = 0, join_partitiontimer = 0;//join during partition.
    uint64_t join_mergetimer_pre = 0, join_mergetimer = 0;//join during merge.
    uint64_t join_timer_pre = 0, join_timer = 0;//join.
    uint64_t shuffle_timer_pre = 0, shuffle_timer = 0;//shuffle.
    std::vector<uint64_t> recordR;
    std::vector<uint64_t> recordS;
    std::vector<uint64_t> recordRID;
    std::vector<uint64_t> recordSID;
    int record_cnt = 0;
    int record_gap = 2000;//TODO: make it consistent with hashing directory.
    int simulate_compute_time = 1;//(microseconds per output).
#endif
};

/** print out the execution time statistics of the join */
void breakdown_global(int64_t result, int nthreads, T_TIMER *timer, long lastTS, _IO_FILE *pFile);

/** print out the execution time statistics of the join */
void breakdown_thread(int64_t result, T_TIMER *timer, long lastTS, _IO_FILE *pFile);

void merge(T_TIMER *timer, relation_t *relR, relation_t *relS, uint64_t *startTS, long lastTS);

void sortRecords(string algo_name, int exp_id, long lastTS, unsigned long inputs, int64_t matches);

#ifndef BEGIN_MEASURE_BUILD
#define BEGIN_MEASURE_BUILD(timer) \
    startTimer(&timer->buildtimer);
#endif

#ifndef END_MEASURE_BUILD
#define END_MEASURE_BUILD(timer) \
    stopTimer(&timer->buildtimer);
#endif

#ifndef /*BEGIN_MEASURE_BUILD_ACC*/NO_TIMING
#define BEGIN_MEASURE_BUILD_ACC(timer) \
    startTimer(&timer->buildtimer_pre);
#endif

#ifndef /*END_MEASURE_BUILD_ACC*/NO_TIMING
#define END_MEASURE_BUILD_ACC(timer) \
     accTimer(&timer->buildtimer_pre, &timer->buildtimer); /* build time */
#endif

#ifndef /*BEGIN_MEASURE_SORT_ACC*/NO_TIMING
#define BEGIN_MEASURE_SORT_ACC(timer) \
    startTimer(&timer->sorttimer_pre);
#endif

#ifndef /*END_MEASURE_SORT_ACC*/NO_TIMING
#define END_MEASURE_SORT_ACC(timer) \
     accTimer(&timer->sorttimer_pre, &timer->sorttimer); /* sort time */
#endif

#ifndef /*BEGIN_MEASURE_JOIN_MERGE_ACC*/NO_TIMING
#define BEGIN_MEASURE_JOIN_MERGE_ACC(timer) \
    startTimer(&timer->join_mergetimer_pre);
#endif

#ifndef /*END_MEASURE_JOIN_MERGE_ACC*/NO_TIMING
#define END_MEASURE_JOIN_MERGE_ACC(timer) \
     accTimer(&timer->join_mergetimer_pre, &timer->join_mergetimer); /* merge time */
#endif


#ifndef /*BEGIN_MEASURE_JOIN_PARTITION_ACC*/NO_TIMING
#define BEGIN_MEASURE_JOIN_PARTITION_ACC(timer) \
    startTimer(&timer->join_partitiontimer_pre);
#endif

#ifndef /*END_MEASURE_JOIN_PARTITION_ACC*/NO_TIMING
#define END_MEASURE_JOIN_PARTITION_ACC(timer) \
     accTimer(&timer->join_partitiontimer_pre, &timer->join_partitiontimer); /* join during partition */
#endif

#ifndef BEGIN_MEASURE_SHUFFLE_ACC
#define BEGIN_MEASURE_SHUFFLE_ACC(timer) \
    startTimer(&timer->shuffle_timer_pre);
#endif

#ifndef END_MEASURE_SHUFFLE_ACC
#define END_MEASURE_SHUFFLE_ACC(timer) \
     accTimer(&timer->shuffle_timer_pre, &timer->shuffle_timer); /* shuffle time */
#endif

#ifndef BEGIN_MEASURE_JOIN_ACC
#define BEGIN_MEASURE_JOIN_ACC(timer) \
    startTimer(&timer->join_timer_pre);
#endif

#ifndef /*END_MEASURE_JOIN_ACC*/NO_TIMING
#define END_MEASURE_JOIN_ACC(timer) \
     accTimer(&timer->join_timer_pre, &timer->join_timer); /* join time */
#endif

#ifndef /*BEGIN_MEASURE_MERGE_ACC*/NO_TIMING
#define BEGIN_MEASURE_MERGE_ACC(timer) \
    startTimer(&timer->mergetimer_pre);
#endif

#ifndef /*END_MEASURE_MERGE_ACC*/NO_TIMING
#define END_MEASURE_MERGE_ACC(timer) \
     accTimer(&timer->mergetimer_pre, &timer->mergetimer); /* merge time */ \
     timer->mergetimer-=timer->join_mergetimer;/*except the join time happen during merge.*/ \
     timer->join_mergetimer=0;
#endif

#ifndef BEGIN_MEASURE_WAIT_ACC
#define BEGIN_MEASURE_WAIT_ACC(timer) \
        startTimer(&timer->wait_timer_pre); /* wait time */
#endif

#ifndef END_MEASURE_WAIT_ACC
#define END_MEASURE_WAIT_ACC(timer) \
      accTimer(&timer->wait_timer_pre, &timer->wait_timer); /* wait time */
#endif

#ifndef SET_WAIT_ACC
#define SET_WAIT_ACC(timer, delay) \
     timer->wait_timer=delay; /* wait time */
#endif

#ifndef /*BEGIN_MEASURE_DEBUILD_ACC*/NO_TIMING
#define BEGIN_MEASURE_DEBUILD_ACC(timer) \
     startTimer(&timer->debuildtimer_pre); /* clean time */
#endif
#ifndef /*END_MEASURE_DEBUILD_ACC*/NO_TIMING
#define END_MEASURE_DEBUILD_ACC(timer) \
     accTimer(&timer->debuildtimer_pre, &timer->debuildtimer); /* clean time */
#endif
#ifndef /*BEGIN_MEASURE_PARTITION_ACC*/NO_TIMING
#define BEGIN_MEASURE_PARTITION_ACC(timer) \
   startTimer(&timer->partition_timer_pre);
#endif

#ifndef /*END_MEASURE_PARTITION_ACC*/NO_TIMING
#define END_MEASURE_PARTITION_ACC(timer) \
   accTimer(&timer->partition_timer_pre, &timer->partition_timer); /* partition time */
#endif

#ifndef /*BEGIN_MEASURE_PARTITION*/NO_TIMING
#define BEGIN_MEASURE_PARTITION(timer) \
   startTimer(&timer->partition_timer);
#endif

#ifndef END_MEASURE_PARTITION
#define END_MEASURE_PARTITION(timer) \
   stopTimer(&timer->partition_timer);
#endif

#ifndef REFINE_MEASURE_PARTITION
#define REFINE_MEASURE_PARTITION(timer) \

#endif


#ifndef START_MEASURE
#define START_MEASURE(timer) \
    gettimeofday(&timer->start, NULL); \
    startTimer(&timer->overall_timer); \
    timer->partition_timer = 0; /* no partitioning */
#endif

#ifndef END_MEASURE
#define END_MEASURE(timer) \
    stopTimer(&timer->overall_timer); /* overall */ \
    gettimeofday(&timer->end, NULL);
#endif

#ifndef END_PROGRESSIVE_MEASURE
#define END_PROGRESSIVE_MEASURE(payloadID, timer, IStupleR)      \
        timer->record_cnt++;                                     \
        if(timer->record_cnt == timer->record_gap){              \
            if(IStupleR){                                        \
                auto ts =curtick();                              \
                timer->recordRID.push_back(payloadID);           \
                timer->recordR.push_back(ts);                    \
            }else{                                               \
                auto ts =curtick();                              \
                timer->recordSID.push_back(payloadID);           \
                timer->recordS.push_back(ts);                    \
                }                                                \
            timer->record_cnt=0;                                 \
        }
#endif

#ifndef DUMMY
#pragma GCC push_options
#pragma GCC optimize ("O0")
#define DUMMY()                             \
    double sum=0;                              \
    for (short d = 0; d < 100; d++) { \
        sum++; \
    }
#endif
#pragma GCC pop_options
#endif //ALLIANCEDB_T_TIMER_H
