#!/bin/bash
#./create_infiles.sh inputFile input_dir numFilesPerDirectory

createInputDirectory() {
  if [ -d "$input_dir" ]; then
    echo "ERROR: Directory $input_dir exists."
    exit 1
  else
    mkdir "$input_dir"
  fi
}

readInputFileAndCreateCountryFiles() {
  wordCounterPerLine=0

  # For every line of the input file we take the fourth word which is the country
  # and if it does not exist in the Countries array, then we add it to the array
  # and we add numFilesPerDirectory files
  if [ ! -e "$input_dir" ]; then
    echo "ERROR: No such directory exists"
    exit 1
  fi

  while read -r line; do
    cd $input_dir
    country=$(echo $line | cut -d " " -f 4)
    if [[ !("${countries[@]}" =~ "${country}") ]]; then
      countries+=($country)
      mkdir $country
      cd $country

      for i in $(eval echo {1..$numFilesPerDirectory}); do
        touch "$country-$i.txt"
      done

      cd ..
    fi
    cd ..
  done < "$inputFile"
}

fillCountriesFilesWithRecords() {
  for country in ${countries[@]};
  do
    counterForRoundRobin=0
    while read -r line;
    do
      cd $input_dir
      countryFromLine=$(echo $line | cut -d " " -f 4)
      if [ "$countryFromLine" == "$country" ];
      then
        cd $countryFromLine
        # Decide the country file with the counterForRoundRobin
        if [ $counterForRoundRobin == $numFilesPerDirectory ];
        then
          counterForRoundRobin=1
        else
          ((counterForRoundRobin++))
        fi

        echo $line >>"$countryFromLine-$counterForRoundRobin.txt"
        cd ..
      fi
      cd ..
    done < "$inputFile"
  done
}

### Main shell code

# Input Variables
inputFile=$1
input_dir=$2
numFilesPerDirectory=$3
# Array to keep countries
countries=()

createInputDirectory
readInputFileAndCreateCountryFiles
fillCountriesFilesWithRecords
