/* Class that will handle all the tasks of the Monitor processes. In order to save
 * time, and since the point of the exercise is the processes-pipes communication,
 * the Monitor class will be similar to the Project1 main.cpp.
 */

#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_MONITOR_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_MONITOR_H

#include "Helper.h"
#include "VaccinationCenter.h"
#include "CitizenRecordsFileReader.h"

class Monitor {
public:
    Monitor(PersonLinkedList *people, VirusLinkedList *viruses, CountryLinkedList *countries,
            VaccinationCenter *vaccinationCenter, CitizenRecordsFileReader *fileReader);

private:
    PersonLinkedList *people;
    VirusLinkedList *viruses;
    CountryLinkedList *countries;
    VaccinationCenter *vaccinationCenter;
    CitizenRecordsFileReader *fileReader;
};


#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_MONITOR_H
