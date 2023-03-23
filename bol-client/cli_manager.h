#pragma once

enum class UIEventType :uint32_t;
class GraphicScene;

struct DeserializePacketWithIdChannel;

class ClientManager
{
public:
	ClientManager(uint32_t id_channel) : id_channel_(id_channel){}

	void initGameUI();

	void handleUIEvent(UIEventType event) const;

	void handlePacket(const DeserializePacketWithIdChannel& packet) const;
private:
	std::shared_ptr<GraphicScene> graphic_scene_;
	uint32_t id_channel_ = 0;
	
	sf::RenderWindow createWindow() const;
};

