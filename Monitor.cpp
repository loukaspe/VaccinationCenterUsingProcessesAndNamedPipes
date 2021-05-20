#include "Monitor.h"

//PersonLinkedList *people = new PersonLinkedList();
//VirusLinkedList *viruses = new VirusLinkedList();
//CountryLinkedList *countries = new CountryLinkedList();
//
//VaccinationCenter *vaccinationCenter = new VaccinationCenter(
//        people,
//        viruses,
//        countries
//);
//
//CitizenRecordsFileReader *fileReader = new CitizenRecordsFileReader(
//        citizenRecordsFile,
//        vaccinationCenter
//);
//
//fileReader->readAndUpdateStructures();

Monitor::Monitor(
        PersonLinkedList *people,
        VirusLinkedList *viruses,
        CountryLinkedList *countries,
        VaccinationCenter *vaccinationCenter,
        CitizenRecordsFileReader *fileReader
) : people(people),
    viruses(viruses),
    countries(countries),
    vaccinationCenter(vaccinationCenter),
    fileReader(fileReader) {

}
