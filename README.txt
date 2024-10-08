Compile:
    * mkdir build
    * cd build
    * cmake ..
    * make
Unit test:
    * make test

Run the App:
./build/mergeApp/mergeApp -f CSCO.txt,MSFT.txt -d 5


Idea:
    -- Read data from each file in d_time interval.
    -- In this way it is easy to merge the data,
    -- and improve sorting. The sorting can be further 
    -- improved (future work).
    -- Read data in a time interval make the multithread
    -- or parallele processing easy. Results of multithread can
    --  be directly merged since each chunck is sorted and in time ordered.
    -- I didn't impelement the multithread this time (leave it for future).