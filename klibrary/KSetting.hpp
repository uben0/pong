#ifndef KSetting_HEADER_DEFINED
#define KSetting_HEADER_DEFINED

#include <string>
#include <fstream>
#include <sstream>

/// \brief A class that manages a variable like a setting.
class KAbstractSetting {
protected:
	KAbstractSetting(const std::string& name) : name(name) {}
public:
	virtual ~KAbstractSetting() {}
	const std::string name;
	virtual bool load(std::istream& file) = 0;
	virtual void save(std::ostream& file, unsigned align = 0) const = 0;
};

template<typename T>
class KSetting : public KAbstractSetting {
public:
	T value;
	KSetting(const std::string& name, const T& value) : KAbstractSetting(name), value(value) {
	}
	virtual bool load(std::istream& file) {
		std::string line;
		file.clear();
		file.seekg(0);
		while (std::getline(file, line)) {
		 	unsigned i = 0;
			while (i < line.size() and i < name.size() and line[i] == name[i]) i++;
			if (i != name.size()) continue;
			while (i < line.size() and std::isspace(line[i])) i++;
			if (i < line.size() and (line[i] == ':' or line[i] == '=')) {
				i++;
				while (i < line.size() and std::isspace(line[i])) i++;
				if (i < line.size()) {
					std::istringstream istream(line.substr(i, line.size()));
					istream >> value;
					return true;
				}
			}
		}
		return false;
	}
	bool load(std::istream& file, const T& min, const T& max) {
		bool result = load(file);
		if (value < min) {
			value = min;
			return false;
		} else
		if (value > max) {
			value = max;
			return false;
		}
		return result;
	}
	virtual void save(std::ostream& file, unsigned align = 0) const {
		file.seekp(0, std::ios::end);
		file << name;
		unsigned space = name.size();
		while (space < align) {
			file << ' ';
			space++;
		}
		file << " : " << value << std::endl;
	}
	virtual ~KSetting() {}
};

template<typename T>
class KSetting <T*> : public KAbstractSetting {
public:
	T* const pointer;
	KSetting(const std::string& name, T* pointer) : KAbstractSetting(name), pointer(pointer) {
	}
	virtual bool load(std::istream& file) {
		std::string line;
		file.clear();
		file.seekg(0);
		while (std::getline(file, line)) {
			unsigned i = 0;
			while (i < line.size() and i < name.size() and line[i] == name[i]) i++;
			if (i != name.size()) continue;
			while (i < line.size() and std::isspace(line[i])) i++;
			if (i < line.size() and (line[i] == ':' or line[i] == '=')) {
				i++;
				while (i < line.size() and std::isspace(line[i])) i++;
				if (i < line.size()) {
					std::istringstream istream(line.substr(i, line.size()));
					istream >> *pointer;
					return true;
				}
			}
		}
		return false;
	}
	bool load(std::istream& file, const T& min, const T& max) {
		bool result = load(file);
		if (*pointer < min) {
			*pointer = min;
			return false;
		} else
		if (*pointer > max) {
			*pointer = max;
			return false;
		}
		return result;
	}
	virtual void save(std::ostream& file, unsigned align = 0) const {
		file.seekp(0, std::ios::end);
		file << name;
		unsigned space = name.size();
		while (space < align) {
			file << ' ';
			space++;
		}
		file << " : " << *pointer << std::endl;
	}
	virtual ~KSetting() {}
};

class KSettingLoad {
	std::istream& mFile;
public:
	KSettingLoad(std::istream& file) : mFile(file) {}
	void operator()(KAbstractSetting& setting) {
		setting.load(mFile);
	}
	void operator()(KAbstractSetting* setting) {
		setting->load(mFile);
	}
};

class KSettingSave {
	std::ostream& mFile;
public:
	KSettingSave(std::ostream& file) : mFile(file) {}
	void operator()(KAbstractSetting& setting) {
		setting.save(mFile);
	}
	void operator()(KAbstractSetting* setting) {
		setting->save(mFile);
	}
};

#endif
