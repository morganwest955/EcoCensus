#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>

int predictions(char* directoryInput, char* directoryOutput){

    std::fstream modifyPythonScript;
    std::ofstream copyPythonScript;
    
    modifyPythonScript.open("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/predictions.py");
    copyPythonScript.open("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/predictions_copy.py");

    std::string line;
    for(int i = 0; i < 7; i++){
        std::getline(modifyPythonScript, line);
        copyPythonScript << line << std::endl;
    }
    std:: stringstream directoryInput1, directoryOutput2;
    std::getline(modifyPythonScript, line);
    directoryInput1 << "rootdir = os.path.dirname('" << directoryInput << "')" << std::setw(80) << std::endl;
    copyPythonScript << directoryInput1.str();
    std::getline(modifyPythonScript, line);
    directoryOutput2 << "directory = os.path.dirname('" << directoryOutput << "')" << std::setw(80) << std::endl;
    copyPythonScript << directoryOutput2.str();
    while(std::getline(modifyPythonScript, line)){
        copyPythonScript << line << std::endl;
    }
    copyPythonScript.close();
    modifyPythonScript.close();
    remove("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/predictions.py");
    rename("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/predictions_copy.py", "/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/predictions.py");
    
    system("cd && source tensorflow/bin/activate && cd /Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017 && python /Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/predictions.py");
    
    return 1;
}

int image_partition(char* directoryInput, char* directoryOutput){

    std::fstream modifyPythonScript;
    std::ofstream copyPythonScript;
    
    modifyPythonScript.open("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/image_partition.py");
    copyPythonScript.open("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/image_partition_copy.py");
    
    std::string line;
    for(int i = 0; i < 11; i++){
        std::getline(modifyPythonScript, line);
        copyPythonScript << line << std::endl;
    }
    std:: stringstream directoryInput1, directoryOutput2;
    std::getline(modifyPythonScript, line);
    directoryInput1 << "rootdir = os.path.dirname('" << directoryInput << "')" << std::setw(80) << std::endl;
    copyPythonScript << directoryInput1.str();
    std::getline(modifyPythonScript, line);
    directoryOutput2 << "directory = os.path.dirname('" << directoryOutput << "')" << std::setw(80) << std::endl;
    copyPythonScript << directoryOutput2.str();
    while(std::getline(modifyPythonScript, line)){
        copyPythonScript << line << std::endl;
    }
    copyPythonScript.close();
    modifyPythonScript.close();
    remove("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/image_partition.py");
    rename("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/image_partition_copy.py", "/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/image_partition.py");
    
    system("cd && source tensorflow/bin/activate && cd /Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017 && python /Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/image_partition.py");
    
    return 1;
}


int train(char* directoryInput){
    
    std::fstream modifyPythonScript;
    std::ofstream copyPythonScript;
    
    modifyPythonScript.open("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/train.py");
    copyPythonScript.open("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/train_copy.py");

    std::string line;
    for(int i = 0; i < 26; i++){
        std::getline(modifyPythonScript, line);
        copyPythonScript << line << std::endl;
    }
    std:: stringstream directoryInput1;
    std::getline(modifyPythonScript, line);
    directoryInput1 << "train_path = os.path.dirname('" << directoryInput << "')" << std::setw(80) << std::endl;
    copyPythonScript << directoryInput1.str();
    while(std::getline(modifyPythonScript, line)){
        copyPythonScript << line << std::endl;
    }
    copyPythonScript.close();
    modifyPythonScript.close();
    remove("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/train.py");
    rename("/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/train_copy.py", "/Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/train.py");
    
    system("cd && source tensorflow/bin/activate && cd /Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017 && python /Users/bound_to_love/Downloads/EcoCensus-master/CurrentImplementation07122017/train.py");
    
    return 1;
}
