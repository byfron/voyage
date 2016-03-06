#include "ServerMapHandler.hpp"
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include <spdlog/spdlog.h>
#include <game/MapRegion.hpp>
#include <memory>

namespace spd = spdlog;
using namespace voyage;

ServerMapHandler::ServerMapHandler(GameServer::Ptr gs) {
	_gameServer = gs;
	_handlerId = SERVER_MAP_REQ_HANDLER;
}

void ServerMapHandler::onMessage(RakNet::Packet *p) {

	switch(p->data[0]) {

	case ID_CS_REGION_REQUEST:

		Message<cs_regionDataRequest>::Ptr m =
			std::make_shared< Message<cs_regionDataRequest> >(p);

		PlayerSession::Ptr pSession = _gameServer->getPlayerSession(m->getAddr());
		handleRegionDataRequest(pSession, m->getContent());
		
		break;
	};
}

void ServerMapHandler::handleRegionDataRequest(PlayerSession::Ptr session, const cs_regionDataRequest & content) {

	//get region and send it back to the client
	int tilex = content.x();
	int tiley = content.y();

	TileMap::Ptr tmap = _gameServer->getWorld()->getTileMap();

	std::cout << "gathering region from tile " << tilex << "," << tiley << std::endl;
	
	MapRegion::Ptr reg = tmap->getMapRegionFromTile(tilex, tiley);
	
	//if the "region" timestamp is old or "dirty", send it together with all the instances

	//if (content.timestamp() < reg->timestamp()) {
	std::vector<uint32_t> tiles = tmap->getRegionTiles(reg);
	sc_regionData msgRegData;
	for (int i = 0; i < tiles.size(); i++)
		msgRegData.add_tiles(tiles[i]);

	msgRegData.set_rows(tmap->getRegionW());
	msgRegData.set_cols(tmap->getRegionH());
	//}

	session->sendMessage<sc_regionData>(std::make_shared< Message<sc_regionData> >(ID_SC_REGION_DATA, msgRegData));
			     
	
	
	//if any instance timestamp is old, send it (i.e deleted tree)
	//after a while, remove the "deleted" instances for good, and mark the region as "dirty"

	
	//send update of the entities of the region (i.e a wandering chicken)

	
	//send also north/west/east/south regions?
}
