LOUKAS PETEINARIS sdi1600134

Important Disclaimers:

1. Classes PipeReader and PipeWriter were copied from previous assignment
    in course "Leitourgika Systhmata" of A.Delis and were adjusted to match
    this assignment's needs.

2. I assume for reasons of simplicity (and according to https://piazza.com/class/kkx2o2gvper2nj?cid=172)
    that bufferSize is bigger or equal than the size of an int. PipeReaders & PipeWriters
    are initialized with bufferSize equal to sizeof INT and then through the pipe
    I pass the actual bufferSize.

3. If the bufferSize of the pipes is smaller than the size of the message, then we transfer
    it in chunks. We read a small chunk of the total bytes until we reach total size
    and then add these bytes to the variable to be returned. For example check
    PipeReader::readStringInChunks();