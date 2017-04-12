#pragma once

class ItemComponent {
};

class InventoryComponent {
public:

	void addItem(uint32_t item_id) {
		item_list.push_back(item_id);
	}
	
	int getActiveWeaponId() {
		return item_list.front();
	}

protected:
	std::vector<uint32_t> item_list;
	
};
