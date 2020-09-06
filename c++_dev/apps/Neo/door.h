#ifndef __NEODAIN_APP_NEO_DOOR_H__
#define __NEODAIN_APP_NEO_DOOR_H__

namespace P_Neodain {

const int32_t OPEN = 1;
const int32_t CLOSE = 0;

struct Door {
	void Open();
	void Close();	
	void ShowState() const;

	void SetName(std::string s) { name = s; }
	std::string GetName() const { return name; } 
	void State(int32_t i);

	int32_t state;
	std::string name;	
};

inline void Door::State(int32_t i) {
	state = i;
}

} // namespace P_Neodain

#endif // __NEODAIN_APP_NEO_DOOR_H__
