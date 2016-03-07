#pragma once
#include <networking/PacketHandler.hpp>
#include "../GameClient.hpp"
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"

using namespace voyage;

class ClientMapHandler  : public PacketHandler {
public:
	ClientMapHandler(GameClient::Ptr c) {
		_handlerId = CLIENT_MAP_HANDLER;
		_client = c;
	}

	void onMessage(RakNet::Packet *p) {

				
		//fill tilemap with region tiles
		Message<sc_regionData>::Ptr m =
			std::make_shared< Message<sc_regionData> >(p);

		sc_regionData content = m->getContent();

		int x = content.topleftx();
		int y = content.toplefty();
		int w = content.cols();
		int h = content.rows();
		std::vector<uint32_t> tiles;
		int size = content.tiles().size();
		assert(size  == w*h);		
		std::copy(content.tiles().begin(), content.tiles().end(), std::back_inserter(tiles));

		std::cout << tiles.size() << w*h << std::endl;
		assert(tiles.size() == w*h);

		for (int i = 0; i < tiles.size(); i++)
			std::cout << tiles[i] << std::endl;
		
		_client->getTileMap()->fill(x, y, w, h, tiles);
	}

private:

	GameClient::Ptr _client;
	
};
