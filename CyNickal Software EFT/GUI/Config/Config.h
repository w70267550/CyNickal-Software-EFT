#include <json.hpp>

using json = nlohmann::json;

class Config
{
public:
	static void Render();

private:
	static std::string getConfigDir();
	static std::string getConfigPath(const std::string& configName);

public:
	static void SaveConfig(const std::string& configName);
	static bool LoadConfig(const std::string& configName);

private:
	static json SerializeCheatConfig();
	static void DeserializeCheatConfig(const json& j);
};