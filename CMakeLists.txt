cmake_minimum_required(VERSION 3.17)
project(VaccinationCenterUsingProcessesAndNamedPipes)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        monitor
        monitor.cpp
        PipeReader.cpp PipeReader.h
        PipeWriter.cpp PipeWriter.h
        Helper.cpp Helper.h
        Monitor.cpp Monitor.h
)

add_executable(travelMonitor
        main.cpp
        BitArray.cpp BitArray.h
        BloomFilter.cpp BloomFilter.h
        Helper.cpp Helper.h
        NotVaccinatedPersonSkipListNode.cpp NotVaccinatedPersonSkipListNode.h
        NotVaccinatedSkipList.cpp NotVaccinatedSkipList.h
        VaccinatedPersonSkipListNode.cpp VaccinatedPersonSkipListNode.h
        VaccinatedSkipList.cpp VaccinatedSkipList.h
        Person.cpp Person.h
        Vaccination.cpp Vaccination.h
        PersonLinkedList.cpp PersonLinkedList.h
        PersonLinkedListNode.cpp PersonLinkedListNode.h
        VirusLinkedList.cpp VirusLinkedList.h
        VirusLinkedListNode.cpp VirusLinkedListNode.h
        MenuPromptCreator.cpp MenuPromptCreator.h
        Virus.cpp Virus.h
        CitizenRecordsFileReader.cpp CitizenRecordsFileReader.h
        Date.cpp Date.h
        VaccinationCenter.cpp VaccinationCenter.h
        Country.cpp Country.h CountryLinkedList.cpp CountryLinkedList.h CountryLinkedListNode.cpp CountryLinkedListNode.h
        TravelMonitor.cpp TravelMonitor.h
        PipeReader.cpp PipeReader.h
        PipeWriter.cpp PipeWriter.h
        )
