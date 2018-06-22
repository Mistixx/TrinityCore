DELETE FROM `battlemaster_list` WHERE `ID`=894;
INSERT INTO `battlemaster_list` VALUES
(894, 'Seething Shore', 'Resource Race', NULL, NULL, 1864730, 1803, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 3, 1, 5, 110, 110, 10, 5, 10, 24, 0);

DELETE FROM `hotfix_data` WHERE `Id`=15047 AND `TableHash`=1435332686 AND `RecordId`=894;
INSERT INTO `hotfix_data` VALUES (15047, 1435332686, 894, 0);
