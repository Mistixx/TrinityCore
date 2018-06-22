-- Dumping structure for table legion_world.battleground_capture_point_creature
DROP TABLE IF EXISTS `battleground_capture_point_creature`;
CREATE TABLE IF NOT EXISTS `battleground_capture_point_creature` (
  `BattlegroundEntry` int(11) NOT NULL,
  `CapturePointKey` int(10) unsigned NOT NULL,
  `SpawnId` bigint(20) unsigned NOT NULL,
  `TeamId` tinyint(3) unsigned NOT NULL DEFAULT '3' COMMENT '0 - Alliance, 1- Horde, 2 - Neutral',
  `Comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`BattlegroundEntry`,`CapturePointKey`,`SpawnId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='Links capture points to creature spawns when a team controls them. Like graveyards';

-- Dumping structure for table legion_world.battleground_capture_point_gameobject
DROP TABLE IF EXISTS `battleground_capture_point_gameobject`;
CREATE TABLE IF NOT EXISTS `battleground_capture_point_gameobject` (
  `BattlegroundEntry` int(11) NOT NULL,
  `CapturePointKey` int(10) unsigned NOT NULL,
  `SpawnId` bigint(20) unsigned NOT NULL,
  `TeamId` tinyint(3) unsigned NOT NULL DEFAULT '3' COMMENT '0 - Alliance, 1- Horde, 2 - Neutral',
  `Comment` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`BattlegroundEntry`,`CapturePointKey`,`SpawnId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='Links capture points to creature spawns when a team controls them. Like graveyards';

-- Dumping structure for table legion_world.battleground_creature
DROP TABLE IF EXISTS `battleground_creature`;
CREATE TABLE IF NOT EXISTS `battleground_creature` (
  `spawnId` bigint(20) unsigned NOT NULL COMMENT 'guid',
  `battlegroundEntry` int(11) unsigned NOT NULL,
  `remove` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spawnId`,`battlegroundEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping structure for table legion_world.battleground_gameobject
DROP TABLE IF EXISTS `battleground_gameobject`;
CREATE TABLE IF NOT EXISTS `battleground_gameobject` (
  `spawnId` bigint(20) unsigned NOT NULL COMMENT 'guid',
  `battlegroundEntry` int(11) unsigned NOT NULL,
  `remove` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spawnId`,`battlegroundEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping structure for table legion_world.battleground_template_score
DROP TABLE IF EXISTS `battleground_template_score`;
CREATE TABLE IF NOT EXISTS `battleground_template_score` (
  `BattlegroundEntry` int(10) unsigned NOT NULL,
  `ScoreType` int(10) unsigned NOT NULL,
  `OrderIndex` int(11) unsigned NOT NULL,
  PRIMARY KEY (`BattlegroundEntry`,`ScoreType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Keeps track of the score types in the battleground';

-- Dumping structure for table legion_world.gameobject_areatrigger
DROP TABLE IF EXISTS `gameobject_areatrigger`;
CREATE TABLE IF NOT EXISTS `gameobject_areatrigger` (
  `Entry` int(10) unsigned NOT NULL,
  `AreaTriggerEntry` int(10) unsigned NOT NULL,
  `Comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Entry`,`AreaTriggerEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='This links gameobjects with area triggers. Intended purpose: battleground buffs';

ALTER TABLE `battleground_template`
  ADD COLUMN `AllianceExploitLoc` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER `Weight`,
  ADD COLUMN `HordeExploitLoc` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER `AllianceExploitLoc`,
  ADD COLUMN `MaxPoints` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0' AFTER `HordeExploitLoc`,
  DROP COLUMN `MinPlayersPerTeam`,
  DROP COLUMN `MaxPlayersPerTeam`,
  DROP COLUMN `MinLvl`,
  DROP COLUMN `MaxLvl`;

-- Dumping structure for table legion_world.battleground_door
DROP TABLE IF EXISTS `battleground_door`;
CREATE TABLE `battleground_door` (
  `BattlegroundEntry` INT(10) UNSIGNED NOT NULL,
  `GameObjectSpawnId` BIGINT(20) UNSIGNED NOT NULL,
  `Comment` VARCHAR(50) NULL DEFAULT NULL,
  PRIMARY KEY (`BattlegroundEntry`, `GameObjectSpawnId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
