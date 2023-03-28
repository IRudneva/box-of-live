#pragma once

enum class UIEventType :uint32_t;
class GraphicScene;

struct ServerPacket;

class ClientManager
{
public:
	void updateGameScene();

	void handleUIEvent(UIEventType event) const;

	void handlePacket(std::shared_ptr<ServerPacket> packet) const;
private:
	std::shared_ptr<GraphicScene> graphic_scene_;
	
	sf::RenderWindow createWindow() const;

};

