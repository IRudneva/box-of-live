#pragma once

enum class UIEventType :uint32_t;
class GraphicScene;

struct ServerPacket;

class ClientManager
{
public:
	void initGameUI();

	void handleUIEvent(UIEventType event) const;

	void handlePacket(ServerPacket& packet) const;
private:
	std::shared_ptr<GraphicScene> graphic_scene_;
	
	sf::RenderWindow createWindow() const;
};

