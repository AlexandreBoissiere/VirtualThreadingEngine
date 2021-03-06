#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

#include "../Memory/memory_decl.h"
#include "variables.h"

variables_decl::variables_decl() {
	this->vars = std::map<std::string, m_container>();
}

void variables_decl::set(std::string var_name, unsigned char value[]) {
	std::map<std::string, m_container>::iterator it;
	it = this->vars.find(var_name);
	if (it != this->vars.end()) {
		return;
	}

	m_container var = m_container();
	var.set(value);
	this->vars[var_name] = var;
}
void variables_decl::set(std::string var_name, unsigned char value[], unsigned long long length) {
	std::map<std::string, m_container>::iterator it;
	it = this->vars.find(var_name);
	if (it != this->vars.end()) {
		return;
	}

	m_container var = m_container();
	var.set(value, length);
	this->vars[var_name] = var;
}

unsigned char* variables_decl::get(std::string var_name) {
	if (!this->vars.count(var_name)) {
		return NULL;
	}
	return this->vars[var_name].get();
}
void variables_decl::deleteVar(std::string var_name) {
	std::map<std::string, m_container>::iterator it;
	it = this->vars.find(var_name);
	if (it != this->vars.end()) {
		this->vars.erase(it);
	}
}

void variables_decl::setVariablesTree(code_file_decl_form branch) {
	if (!this->variables_tree.count(branch.decl_name)) {
		this->variables_tree[branch.decl_name] = branch;
		this->variables_vector.push_back(branch);
	}
}
code_file_decl_form variables_decl::getVariableBranch(std::string var_name) {
	if (this->variables_tree.count(var_name)) {
		return this->variables_tree[var_name];
	}
	else {
		code_file_decl_form err;
		err.decl_attr = "undefined";
		err.decl_type = "undefined";
		err.decl_name = "<parsing-error>";
		err.decl_value = "<parsing-error>";
		return err;
	}
}
std::vector<code_file_decl_form> variables_decl::getVariablesTree() {
	return this->variables_vector;
}

void variables_decl::setTag(std::string tagname, unsigned long long value) {
	this->tags[tagname] = value;
}
unsigned long long variables_decl::getTag(std::string tagname) {
	if (this->tags.count(tagname)) {
		return this->tags[tagname];
	}
	return NULL;
}
void variables_decl::setTagsMap(std::map<std::string, unsigned long long> map) {
	this->tags = map;
}
std::map<std::string, unsigned long long> variables_decl::getTagsMap() {
	return this->tags;
}

void variables_decl::setTagsBranch(tag_decl_form branch) {
	if (!this->tags_tree.count(branch.tagname)) {
		this->tags_tree[branch.tagname] = branch;
		this->tags_vector.push_back(branch);
	}
}
tag_decl_form variables_decl::getTagBranch(std::string tagname) {
	if (this->tags_tree.count(tagname)) {
		return this->tags_tree[tagname];
	}
	else {
		tag_decl_form err;
		err.tagname = "undefined";
		err.value = NULL;
		return err;
	}
}
std::vector<tag_decl_form> variables_decl::getTagsTree() {
	return this->tags_vector;
}

code_file_decl_form processDeclCodeLine(std::string line) {
	code_file_decl_form decl_data;

	if (line.substr(5, 6) == "string") {
		decl_data.decl_attr = "defined";
		decl_data.decl_type = "string";
		
		std::stringstream ss(line.substr(12));
		
		if (std::getline(ss, decl_data.decl_name, ' ')) {
			if (!std::getline(ss, decl_data.decl_value)) {
				decl_data.decl_value = "<parsing-error>";
			}
		}
		else {
			decl_data.decl_name = "<parsing-error>";
		}

		return decl_data;
	}
	else if (line.substr(5, 15) == "unsigned_number") {
		decl_data.decl_attr = "defined";
		decl_data.decl_type = "unsigned number";

		std::stringstream ss(line.substr(21));

		ss >> decl_data.decl_name >> std::ws >> decl_data.decl_value;
	}
	else if (line.substr(5, 13) == "signed_number") {
		decl_data.decl_attr = "defined";
		decl_data.decl_type = "signed number";

		std::stringstream ss(line.substr(19));

		ss >> decl_data.decl_name >> std::ws >> decl_data.decl_value;
	}
	else {
		decl_data.decl_attr = "undefined";
		decl_data.decl_type = "undefined";
		decl_data.decl_name = "<parsing-error>";
		decl_data.decl_value = "<parsing-error>";
	}
	return decl_data;
}

std::vector<code_file_decl_form> parse_code_file(std::string filename, std::vector<tag_decl_form>* tagsvec) {
	std::ifstream file(filename);
	std::vector<code_file_decl_form> decl_lines;
	std::string line = "";

	if (file) {
		while (!file.eof()) {
			std::getline(file, line);

			if (line.empty() || line[0] == ';') {
				continue;
			}
			else if (!line.rfind("decltag ", 0)) {
				tag_decl_form tag;
				tag.tagname = line.substr(8);
				tag.value = NULL;
				tagsvec->push_back(tag);
			}
			else if (!line.rfind("decl ", 0)) {
				decl_lines.push_back(processDeclCodeLine(line));
			}
			continue;
		}
	}
	else {
		std::cout << "Unable to read secified code file" << std::endl;
		return decl_lines;
	}

	return decl_lines;
}

variables_decl build_variables_decl_tree(std::string filename) {
	variables_decl storage = variables_decl();
	std::vector<tag_decl_form> tagsvec;
	std::vector<code_file_decl_form> parsed = parse_code_file(filename, &tagsvec);

	for (unsigned long long i = 0; i < parsed.size(); i++) {
		if (parsed[i].decl_attr == "defined") {
			if (parsed[i].decl_type == "undefined" || parsed[i].decl_name == "<parsing-error>" ||
				parsed[i].decl_value == "<parsing-error>") {
				continue;
			}
			else {
				if (parsed[i].decl_type == "string") {
					unsigned char* uc_s = new unsigned char[parsed[i].decl_value.size() + 1];
					memcpy_s(uc_s, parsed[i].decl_value.size() + 1, parsed[i].decl_value.c_str(), parsed[i].decl_value.size() + 1);
					storage.set(parsed[i].decl_name, uc_s, parsed[i].decl_value.size() + 1);
					storage.setVariablesTree(parsed[i]);
				}
				else if (parsed[i].decl_type == "unsigned number") {
					std::stringstream ss(parsed[i].decl_value);
					unsigned long long n_value;
					ss >> n_value;
					storage.set(parsed[i].decl_name, (unsigned char*)n_value);
					storage.setVariablesTree(parsed[i]);
				}
				else if (parsed[i].decl_type == "signed number") {
					std::stringstream ss(parsed[i].decl_value);
					long long n_value;
					ss >> n_value;
					storage.setVariablesTree(parsed[i]);
				}
			}
		}
		else {
			continue;
		}
	}
	for (unsigned long long i = 0; i < tagsvec.size(); i++) {
		if (tagsvec[i].tagname == "undefined") {
			continue;
		}
		storage.setTag(tagsvec[i].tagname, tagsvec[i].value);
		//storage.setTagsBranch(tagsvec[i]);
	}

	return storage;
}