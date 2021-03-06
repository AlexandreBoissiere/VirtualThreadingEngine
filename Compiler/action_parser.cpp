#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

#include "../Actions/threading.h"
#include "../Memory/memory_decl.h"
#include "variables.h"
#include "action_parser.h"

void process_memory::set(variables_decl* var) {
	std::vector<code_file_decl_form> headers = var->getVariablesTree();

	for (unsigned long long i = 0; i < headers.size(); i++) {
		if (headers[i].decl_type == "string") {
			this->stored_strings[headers[i].decl_name] = headers[i].decl_value;
			this->data_ptrs[headers[i].decl_name] = &this->stored_strings[headers[i].decl_name];
		}
		else if (headers[i].decl_type == "unsigned number") {
			unsigned long long value;
			std::stringstream ss(headers[i].decl_value);
			ss >> value;
			this->unsigned_numbers[headers[i].decl_name] = value;
			this->data_ptrs[headers[i].decl_name] = &this->unsigned_numbers[headers[i].decl_name];
		}
		else if (headers[i].decl_type == "signed number") {
			long long value;
			std::stringstream ss(headers[i].decl_value);
			ss >> value;
			this->signed_numbers[headers[i].decl_name] = value;
			this->data_ptrs[headers[i].decl_name] = &this->signed_numbers[headers[i].decl_name];
		}
	}
}
void process_memory::setTags(variables_decl* vars) {
	std::vector<tag_decl_form> tags_headers = vars->getTagsTree();

	for (unsigned long long i = 0; i < tags_headers.size(); i++) {
		this->stored_tags[tags_headers[i].tagname] = tags_headers[i].value;
		this->data_ptrs[tags_headers[i].tagname] = &this->stored_tags[tags_headers[i].tagname];
	}
}
void process_memory::setTagValue(std::string tagname, unsigned long long value) {
	this->stored_tags[tagname] = value;
}
void process_memory::set(std::string var_name, void* data_ptr) {
	if (!this->data_ptrs.count(var_name)) {
		this->data_ptrs[var_name] = data_ptr;
	}
}
void process_memory::setRegisters() {
	this->data_ptrs["AX"] = (void*)registries_def::AX;
	this->data_ptrs["BX"] = (void*)registries_def::BX;
	this->data_ptrs["CX"] = (void*)registries_def::CX;
	this->data_ptrs["DX"] = (void*)registries_def::DX;

	this->data_ptrs["EAX"] = (void*)registries_def::EAX;
	this->data_ptrs["EBX"] = (void*)registries_def::EBX;
	this->data_ptrs["ECX"] = (void*)registries_def::ECX;
	this->data_ptrs["EDX"] = (void*)registries_def::EDX;

	this->data_ptrs["RAX"] = (void*)registries_def::RAX;
	this->data_ptrs["RBX"] = (void*)registries_def::RBX;
	this->data_ptrs["RCX"] = (void*)registries_def::RCX;
	this->data_ptrs["RDX"] = (void*)registries_def::RDX;

	this->data_ptrs["SR"] = (void*)extra_registries::SR;
}
bool process_memory::isTag(std::string tagname) {
	if (this->stored_tags.count(tagname)) {
		return true;
	}
	else {
		return false;
	}
}

void* process_memory::getVarPtr(std::string var_name) {
	if (this->data_ptrs.count(var_name)) {
		return this->data_ptrs[var_name];
	}
	else {
		std::cout << "Warning: Symbol '" << var_name << "' unrecognized, replaced by NULL statement" << std::endl;
		return NULL;
	}
}

std::map<std::string, virtual_actions> symbols_converter =
{
#pragma region map_decl
	{"getAX", virtual_actions::getAX},
	{"getBX", virtual_actions::getBX},
	{"getCX", virtual_actions::getCX},
	{"getDX", virtual_actions::getDX},

	{"setAX", virtual_actions::setAX},
	{"setBX", virtual_actions::setBX},
	{"setCX", virtual_actions::setCX},
	{"setDX", virtual_actions::setDX},

	{"getEAX", virtual_actions::getEAX},
	{"getEBX", virtual_actions::getEBX},
	{"getECX", virtual_actions::getECX},
	{"getEDX", virtual_actions::getEDX},

	{"setEAX", virtual_actions::setEAX},
	{"setEBX", virtual_actions::setEBX},
	{"setECX", virtual_actions::setECX},
	{"setEDX", virtual_actions::setEDX},

	{"getRAX", virtual_actions::getRAX},
	{"getRBX", virtual_actions::getRBX},
	{"getRCX", virtual_actions::getRCX},
	{"getRDX", virtual_actions::getRDX},

	{"setRAX", virtual_actions::setRAX},
	{"setRBX", virtual_actions::setRBX},
	{"setRCX", virtual_actions::setRCX},
	{"setRDX", virtual_actions::setRDX},

	{"setSR", virtual_actions::setSR},
	{"getSR", virtual_actions::getSR},

	{"movAX", virtual_actions::movAX},
	{"movBX", virtual_actions::movBX},
	{"movCX", virtual_actions::movCX},
	{"movDX", virtual_actions::movDX},

	{"movEAX", virtual_actions::movEAX},
	{"movEBX", virtual_actions::movEBX},
	{"movECX", virtual_actions::movECX},
	{"movEDX", virtual_actions::movEDX},

	{"movRAX", virtual_actions::movRAX},
	{"movRBX", virtual_actions::movRBX},
	{"movRCX", virtual_actions::movRCX},
	{"movRDX", virtual_actions::movRDX},

	{"inc", virtual_actions::inc},
	{"dec", virtual_actions::dec},

	{"mulAX", virtual_actions::mulAX},
	{"mulBX", virtual_actions::mulBX},
	{"mulCX", virtual_actions::mulCX},
	{"mulDX", virtual_actions::mulDX},

	{"mulEAX", virtual_actions::mulEAX},
	{"mulEBX", virtual_actions::mulEBX},
	{"mulECX", virtual_actions::mulECX},
	{"mulEDX", virtual_actions::mulEDX},

	{"mulRAX", virtual_actions::mulRAX},
	{"mulRBX", virtual_actions::mulRBX},
	{"mulRCX", virtual_actions::mulRCX},
	{"mulRDX", virtual_actions::mulRDX},

	{"divAX", virtual_actions::divAX},
	{"divBX", virtual_actions::divBX},
	{"divCX", virtual_actions::divCX},
	{"divDX", virtual_actions::divDX},

	{"divEAX", virtual_actions::divEAX},
	{"divEBX", virtual_actions::divEBX},
	{"divECX", virtual_actions::divECX},
	{"divEDX", virtual_actions::divEDX},

	{"divRAX", virtual_actions::divRAX},
	{"divRBX", virtual_actions::divRBX},
	{"divRCX", virtual_actions::divRCX},
	{"divRDX", virtual_actions::divRDX},

	{"addAX", virtual_actions::addAX},
	{"addBX", virtual_actions::addBX},
	{"addCX", virtual_actions::addCX},
	{"addDX", virtual_actions::addDX},

	{"addEAX", virtual_actions::addEAX},
	{"addEBX", virtual_actions::addEBX},
	{"addECX", virtual_actions::addECX},
	{"addEDX", virtual_actions::addEDX},

	{"addRAX", virtual_actions::addRAX},
	{"addRBX", virtual_actions::addRBX},
	{"addRCX", virtual_actions::addRCX},
	{"addRDX", virtual_actions::addRDX},

	{"subAX", virtual_actions::subAX},
	{"subBX", virtual_actions::subBX},
	{"subCX", virtual_actions::subCX},
	{"subDX", virtual_actions::subDX},

	{"subEAX", virtual_actions::subEAX},
	{"subEBX", virtual_actions::subEBX},
	{"subECX", virtual_actions::subECX},
	{"subEDX", virtual_actions::subEDX},

	{"subRAX", virtual_actions::subRAX},
	{"subRBX", virtual_actions::subRBX},
	{"subRCX", virtual_actions::subRCX},
	{"subRDX", virtual_actions::subRDX},

	{"toString", virtual_actions::toString},
	{"mergeString", virtual_actions::mergeString},
	{"substring", virtual_actions::substring},
	{"strlen", virtual_actions::_strlen},
	{"rdstdin", virtual_actions::rdstdin},
	{"print", virtual_actions::print},
	{"println", virtual_actions::println},
	{"printEOL", virtual_actions::printEOL},

	{"castreg", virtual_actions::castreg},
	{"recast", virtual_actions::recast},
	{"fromString", virtual_actions::fromString},

	{"push", virtual_actions::push},
	{"pop", virtual_actions::pop},

	{"pushSR", virtual_actions::pushSR},
	{"popSR", virtual_actions::popSR},

	{"declArray", virtual_actions::declArray},
	{"setAt", virtual_actions::setAt},
	{"getAt", virtual_actions::getAt},
	{"getDynSize", virtual_actions::getDynSize},

	{"dyndecl", virtual_actions::dyndecl},
	{"dynset", virtual_actions::dynset},
	{"dynget", virtual_actions::dynget},

	{"ijmp", virtual_actions::ijmp},
	{"jmp", virtual_actions::jmp},
	{"cmp", virtual_actions::cmp},
	{"je", virtual_actions::je},
	{"jne", virtual_actions::jne},
	{"jl", virtual_actions::jl},
	{"jg", virtual_actions::jg},
	{"jle", virtual_actions::jle},
	{"jge", virtual_actions::jge},
	{"cmpstr", virtual_actions::cmpstr},

	{"gca", virtual_actions::gca},
	{"hlt", virtual_actions::hlt},

	{"call", virtual_actions::call},
	{"lcall", virtual_actions::lcall},
	{"ret", virtual_actions::ret}
#pragma endregion
};

std::vector<std::vector<std::string>> parseCodeLines(std::string filename) {
	std::ifstream file(filename);
	std::vector<std::vector<std::string>> parsed;
	std::string line;

	if (file) {
		while (!file.eof()) {
			std::getline(file, line);

			if (line.empty() || line[0] == ';') {
				continue;
			}

			if (!line.rfind("decltag ", 0)) {
				std::string tagname = line.substr(8);
				parsed.push_back(std::vector<std::string>({"decltag", tagname}));
				continue;
			}

			std::stringstream ss(line);
			std::string action, argument;
			ss >> action >> std::ws >> argument;

			parsed.push_back(std::vector<std::string>({action, argument}));
		}
		return parsed;
	}
	else {
		std::cout << "Unable to read secified code file" << std::endl;
		return parsed;
	}
}
std::vector<virtual_actions> convertSymbols(std::vector<std::vector<std::string>> parsed) {
	std::vector<virtual_actions> converted;

	for (unsigned long long i = 0; i < parsed.size(); i++) {
		converted.push_back(symbols_converter[parsed[i][0]]);
	}

	return converted;
}
void purgeParsed(std::vector<virtual_actions> *converted, std::vector<std::vector<std::string>> *parsed) {
	for (unsigned long long i = 0; i < converted->size(); i++) {
		if ((*converted)[i] == 0 || (*converted)[i] < virtual_actions::setAX) {
			parsed->erase(parsed->begin() + i);
			converted->erase(converted->begin() + i);
			i--;
		}
	}
}

std::vector<std::vector<std::string>> makeCleanedParsed(std::string filename) {
	std::ifstream file(filename);
	std::vector<std::vector<std::string>> parsed;
	std::string line;

	if (file) {
		while (!file.eof()) {
			std::getline(file, line);

			if (line.empty() || line[0] == ';') {
				continue;
			}

			if (!line.rfind("decltag ", 0)) {
				std::string tagname = line.substr(8);
				parsed.push_back(std::vector<std::string>({"decltag", tagname}));
				continue;
			}
			if (!line.rfind("decl ", 0)) {
				continue;
			}

			std::stringstream ss(line);
			std::string action, argument;
			ss >> action >> std::ws >> argument;

			parsed.push_back(std::vector<std::string>({action, argument}));
		}
		return parsed;
	}
	else {
		std::cout << "Unable to read secified code file" << std::endl;
		return parsed;
	}
}

void finalizeTags(std::vector<std::vector<std::string>> cleaned_parsed, variables_decl* vars) {
	for (unsigned long long i = 0; i < cleaned_parsed.size(); i++) {
		if (cleaned_parsed[i][0] == "decltag") {
			vars->setTag(cleaned_parsed[i][1], i - 1);

			tag_decl_form decl_form;
			decl_form.tagname = cleaned_parsed[i][1];
			decl_form.value = i - 1;

			vars->setTagsBranch(decl_form);
			cleaned_parsed.erase(cleaned_parsed.begin() + i);
			i--;
		}
		if (symbols_converter[cleaned_parsed[i][0]] == 0 || symbols_converter[cleaned_parsed[i][0]]  < virtual_actions::setAX) {
			cleaned_parsed.erase(cleaned_parsed.begin() + i);
			i--;
		}
	}
}
std::vector<void*> convertVariables(std::vector<std::vector<std::string>> cleaned_parsed, process_memory* p_mem) {
	std::vector<void*> arguments;

	for (unsigned long long i = 0; i < cleaned_parsed.size(); i++) {
		if (cleaned_parsed[i][1] == "0" || cleaned_parsed[i][1] == "NULL") {
			arguments.push_back(NULL);
		}
		else {
			arguments.push_back(p_mem->getVarPtr(cleaned_parsed[i][1]));
		}
	}

	return arguments;
}

void build_process(std::string filename, process* out_proc, engine* engine, process_memory* out_mem) {
	variables_decl vars = build_variables_decl_tree(filename);
	std::vector<std::vector<std::string>> parsed = parseCodeLines(filename);
	std::vector<virtual_actions> converted_actions = convertSymbols(parsed);

	purgeParsed(&converted_actions, &parsed);
	finalizeTags(makeCleanedParsed(filename), &vars);

	out_mem->set(&vars);
	out_mem->setRegisters();
	out_mem->setTags(&vars);

	std::vector<void*> converted_arguments = convertVariables(parsed, out_mem);

	for (unsigned long long i = 0; i < converted_actions.size(); i++) {
		out_proc->addAction(converted_actions[i], converted_arguments[i]);
	}
}