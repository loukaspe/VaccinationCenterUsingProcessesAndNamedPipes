LOUKAS PETEINARIS sdi1600134

Important Disclaimers:
0. Due to employment reasons I did not invest as much time as I wanted. The app
    is not working. I have only implemented the creation of Monitor processes,
    the passage of Countries' data through the Pipes, the reading of the files,
    and the passage of BloomFilters back to the mother process (last part not
    working-it's blocking). Also the script is implemented. Please be generous in grading me.

1. The code consists of various classes and the code is (at my best effort) self-
    explanatory. Due to lack of time, this was not made everywhere, and in these
    cases I have added according comments to help you understand.

2. All classes except Monitor, TravelMonitor, PipeReader, PipeWriter and
    BloomFilterLinkedList were copied from Project1, and only slightly changed for
    easier handling in this assignment.

3. Classes PipeReader and PipeWriter were copied from previous assignment
    in course "Leitourgika Systhmata" of A.Delis and were adjusted to match
    this assignment's needs. We have function writeBloomFilterInChunks() and

    writeStringInChunks() that are used for handling every bufferSize. Specifically,
    these function read data of size bufferSize from the Pipes and then copy bytes
    the final variable.

4. I assume for reasons of simplicity (and according to https://piazza.com/class/kkx2o2gvper2nj?cid=172)
    that bufferSize is bigger or equal than the size of an int. PipeReaders & PipeWriters
    are initialized with bufferSize equal to sizeof INT and then through the pipe
    I pass the actual bufferSize.

5. If the bufferSize of the pipes is smaller than the size of the message, then we transfer
    it in chunks. We read a small chunk of the total bytes until we reach total size
    and then add these bytes to the variable to be returned. For example check
    PipeReader::readStringInChunks();

6. Protocol for passing subdirectories names to Monitors:
    a. We pass the number of subdirectories' names that are going to be passed
    b. foreach subdirectory's name we pass firstly it's size, then the name itself

7. For quickly combining the Project1's design with this one I add to BF the Virus
    and Country that concern it. So that the TravelMonitor has a LinkedList of BF
    and finds the appropriate one by the Virus and Country. This was the fastest way
    as I did not want to lose much time adjusting the Project1 code and I wanted to
    concentrate to Pipes and Signals.

8. For passing the BloomFilters from Monitors to TravelMonitor the protocol is:
    a. Pass the number of BloomFilter per Monitor
    b. Until all expected BloomFilters arrive, use poll() to read from multiple
        Monitors, whatever comes first.

9. Monitor class was supposed to be used for handling the queries of the users, but
    I never got to this part. BloomFilterLinkedList is a linked list with BloomFilter
    to support the queries in the TravelMonitor. It has a function findByVirusAndCountry(),
    that was going to return the appropriate BloomFilter for searching.
    For example, in /travelRequest I would find the BF with virusName and countrFrom
    names and search in it before going to the Monitor.