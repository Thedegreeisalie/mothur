/*
 *  setoutdircommand.cpp
 *  Mothur
 *
 *  Created by westcott on 1/21/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "setdircommand.h"

//**********************************************************************************************************************

SetDirectoryCommand::SetDirectoryCommand(string option)  {
	try {
		abort = false;
		
		//allow user to run help
		if(option == "help") { help(); abort = true; }
		
		else {
			//valid paramters for this command
			string Array[] =  {"output","input","tempdefault","outputdir","inputdir"};
			vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			//check to make sure all parameters are valid for command
			for (map<string,string>::iterator it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
		
			output = validParameter.validFile(parameters, "output", false);			
			if (output == "not found") {  output = "";  } 
			
			input = validParameter.validFile(parameters, "input", false);			
			if (input == "not found") {  input = "";  }
			
			tempdefault = validParameter.validFile(parameters, "tempdefault", false);			
			if (tempdefault == "not found") {  tempdefault = "";  }
				
			if ((input == "") && (output == "") && (tempdefault == "")) {	
				m->mothurOut("You must provide either an input, output or tempdefault for the set.outdir command."); m->mothurOutEndLine(); abort = true;
			}
		}
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "SetDirectoryCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

void SetDirectoryCommand::help(){
	try {
		m->mothurOut("The set.dir command can be used to direct the output files generated by mothur to a specific place, the directory must exist.\n");
		m->mothurOut("The set.dir command can also be used to specify the directory where your input files are located, the directory must exist.\n");
		m->mothurOut("The set.dir command can also be used to override or set the default location mothur will look for files if it is unable to find them, the directory must exist.\n");
		m->mothurOut("The set.dir command parameters are input, output and tempdefault and one is required.\n");
		m->mothurOut("To return the output to the same directory as the input files you may enter: output=clear.\n");
		m->mothurOut("To return the input to the current working directory you may enter: input=clear.\n");
		m->mothurOut("To set the output to the directory where mothur.exe is located you may enter: output=default.\n");
		m->mothurOut("To set the input to the directory where mothur.exe is located you may enter: input=default.\n");
		m->mothurOut("To return the tempdefault to the default you provided at compile time you may enter: tempdefault=clear.\n");
		m->mothurOut("To set the tempdefault to the directory where mothur.exe is located you may enter: tempdefault=default.\n");
		m->mothurOut("The set.dir command should be in the following format: set.dir(output=yourOutputDirectory, input=yourInputDirectory, tempdefault=yourTempDefault).\n");
		m->mothurOut("Example set.outdir(output=/Users/lab/desktop/outputs, input=/Users/lab/desktop/inputs).\n");
		m->mothurOut("Note: No spaces between parameter labels (i.e. output), '=' and parameters (i.e.yourOutputDirectory).\n\n");
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "help");
		exit(1);
	}
}
//**********************************************************************************************************************

SetDirectoryCommand::~SetDirectoryCommand(){}

//**********************************************************************************************************************

int SetDirectoryCommand::execute(){
	try {
		
		if (abort == true) { return 0; }
		
		commandFactory = CommandFactory::getInstance();
		
		//redirect output
		if ((output == "clear") || (output == "")) {  output = "";  commandFactory->setOutputDirectory(output);  }
		else if (output == "default") { 
			GlobalData* globaldata = GlobalData::getInstance();
			string exepath = globaldata->argv;
			output = exepath.substr(0, (exepath.find_last_of('m')));
			
			m->mothurOut("Changing output directory to " + output); m->mothurOutEndLine();  
			commandFactory->setOutputDirectory(output);
		}else {
			//add / to name if needed
			string lastChar = output.substr(output.length()-1);
			#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux)
				if (lastChar != "/") { output += "/"; }
			#else
				if (lastChar != "\\") { output += "\\"; }	
			#endif
			
			//test to make sure directory exists
			output = m->getFullPathName(output);
			string outTemp = output + "temp";
			ofstream out;
			out.open(outTemp.c_str(), ios::trunc);
			if(!out) {
				m->mothurOut(output + " directory does not exist or is not writable."); m->mothurOutEndLine(); 
			}else{
				out.close();
				remove(outTemp.c_str());
				m->mothurOut("Changing output directory to " + output); m->mothurOutEndLine();  
				commandFactory->setOutputDirectory(output);
			}
		}
		
		//redirect input
		if ((input == "clear") || (input == "")) {  input = "";  commandFactory->setInputDirectory(input);  }
		else if (input == "default") { 
			GlobalData* globaldata = GlobalData::getInstance();
			string exepath = globaldata->argv;
			input = exepath.substr(0, (exepath.find_last_of('m')));
			
			m->mothurOut("Changing input directory to " + input); m->mothurOutEndLine();  
			commandFactory->setInputDirectory(input);
		}else {
			//add / to name if needed
			string lastChar = input.substr(input.length()-1);
			#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux)
				if (lastChar != "/") { input += "/"; }
			#else
				if (lastChar != "\\") { input += "\\"; }	
			#endif
			
			//test to make sure directory exists
			input = m->getFullPathName(input);
			string inTemp = input + "temp";
			ofstream in;
			in.open(inTemp.c_str(), ios::trunc);
			if(!in) {
				m->mothurOut(input + " directory does not exist or is not writable."); m->mothurOutEndLine(); 
			}else{
				in.close();
				remove(inTemp.c_str());
				m->mothurOut("Changing input directory to " + input); m->mothurOutEndLine();  
				commandFactory->setInputDirectory(input); 
			}
		}
		
		//set default
		if (tempdefault == "clear") {  
			#ifdef MOTHUR_FILES
				string temp = MOTHUR_FILES; 
				m->mothurOut("Resetting default directory to " + temp); m->mothurOutEndLine();  
				m->setDefaultPath(temp);
			#else
				string temp = ""; 
				m->mothurOut("No default directory defined at compile time."); m->mothurOutEndLine();  
				m->setDefaultPath(temp);
			#endif
		}else if (tempdefault == "") {  //do nothing
		}else if (tempdefault == "default") { 
			GlobalData* globaldata = GlobalData::getInstance();
			string exepath = globaldata->argv;
			tempdefault = exepath.substr(0, (exepath.find_last_of('m')));
			
			m->mothurOut("Changing default directory to " + tempdefault); m->mothurOutEndLine();  
			m->setDefaultPath(tempdefault);
		}else {
			//add / to name if needed
			string lastChar = tempdefault.substr(tempdefault.length()-1);
			#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux)
				if (lastChar != "/") { tempdefault += "/"; }
			#else
				if (lastChar != "\\") { tempdefault += "\\"; }	
			#endif
			
			m->mothurOut("Changing default directory to " + tempdefault); m->mothurOutEndLine();  
			m->setDefaultPath(tempdefault);
		}

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************/
