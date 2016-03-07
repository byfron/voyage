#pragma once

/**
 * Interface that replicates the server database contents 
 * - Should load at start-up when logging the high-level tables
 * - Caches region instances that are queried to the server
 * - Provides updated quick-access to data to the interface
 * - Should cache only the stuff that is present in the scene, and keep it 
 * for some time.
 * - The large data like images can be kept and updated only when it changes.
 * - Should keep small copies of the main dataset
 */
class ClientDatabaseCacher {

public:
	DatabaseResult runSELECTQuery(std::string query);
	int createDatasetFromServerQuery(MessageSCDatabaseDownload*);
	int updateDatasetFromServerQuery(MessageSCUpdateRegionData*);

private:

	int _runUPDATEQuery(std::string query);
	
};
