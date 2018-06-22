
UPDATE `creature_template_addon` SET `auras`='22011 9036 9617' WHERE  `entry`=13116;
UPDATE `creature_template_addon` SET `auras`='22011 9036 9617' WHERE  `entry`=13117;

DELETE FROM `gameobject_template` WHERE `entry` IN (227744 /*Horde Flag*/, 227745 /*Alliance Flag*/, 227741 /*Alliance Flag*/, 227740 /*Horde Flag*/);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `RequiredLevel`, `VerifiedBuild`) VALUES
(227744, 37, 5913, 'Horde Flag', '', '', '', 2.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Flag
(227745, 37, 5912, 'Alliance Flag', '', '', '', 2.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Flag
(227741, 36, 5912, 'Alliance Flag', '', '', '', 2.5, 0, 156621, 0, 1, 0, 1, 0, 10000, 10000, 227745, 1, 1545, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Flag
(227740, 36, 5913, 'Horde Flag', '', '', '', 2.5, 0, 156618, 0, 1, 0, 1, 0, 10000, 10000, 227744, 1, 1546, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- Horde Flag

DELETE FROM `gameobject_template_addon` WHERE `entry` IN (227744 /*Horde Flag*/, 227745 /*Alliance Flag*/, 227741 /*Alliance Flag*/, 227740 /*Horde Flag*/);
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`) VALUES
(227744, 2058, 0), -- Horde Flag
(227745, 1913, 0), -- Alliance Flag
(227741, 1995, 32), -- Alliance Flag
(227740, 1997, 32); -- Horde Flag

UPDATE `gameobject_template_addon` SET `flags`=32 WHERE `entry`=179907; -- Berserk Buff
UPDATE `gameobject_template_addon` SET `flags`=32 WHERE `entry`=179905; -- Berserk Buff
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=179921; -- Doodad_PortcullisActive04
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=179920; -- Doodad_PortcullisActive03
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=179919; -- Doodad_PortcullisActive02
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=179918; -- Doodad_PortcullisActive01
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=179917; -- Doodad_RazorfenDoor02
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=179916; -- Doodad_RazorfenDoor01

DELETE FROM `gameobject_areatrigger`;
INSERT INTO `gameobject_areatrigger` (`Entry`, `AreaTriggerEntry`, `Comment`) VALUES
(179871, 3686, NULL),
(179871, 5906, 'Twin Peaks - Speed Buff - Alliance'),
(179899, 3687, NULL),
(179899, 5907, 'Twin Peaks - Speed Buff - Horde'),
(179904, 3706, NULL),
(179904, 5909, 'Twin Peaks - Restoration Buff - Alliance'),
(179905, 3707, NULL),
(179905, 5911, 'Twin Peaks - Berserk Buff - Alliance'),
(179906, 3708, NULL),
(179906, 5908, 'Twin Peaks - Restoration Buff - Horde'),
(179907, 3709, NULL),
(179907, 5910, 'Twin Peaks - Berserk Buff - Horde'),
(180144, 3866, NULL),
(180145, 3867, NULL),
(180146, 3868, 'Arathi Basin - Speed Buff - Lumber Mill'),
(180145, 6268, 'Battle for Gilneas - Food Buff - Mines'),
(180146, 6267, 'Battle for Gilneas - Speed Buff - Waterworks 1'),
(180147, 3869, NULL),
(180147, 6266, 'Battle for Gilneas - Speed Buff - Waterworks 2'),
(180148, 6265, 'Battle for Gilneas - Berserk Buff - Waterworks 3'),
(180362, 3869, NULL),
(180376, 3869, NULL),
(180378, 3870, NULL),
(180379, 3866, NULL),
(180379, 6269, 'Battle for Gilneas - Speed Buff - Lighthouse'),
(180380, 3866, NULL),
(180380, 6269, 'Battle for Gilneas - Berserk Buff - Lighthouse'),
(180381, 3867, NULL),
(180378, 3868, 'Arathi Basin - Berserk Buff - Lumber Mill'),
(180381, 6268, 'Battle for Gilneas - Speed Buff - Mines'),
(180382, 3867, NULL),
(180377, 3868, 'Arathi Basin - Food Buff - Lumber Mill'),
(180382, 6268, 'Battle for Gilneas - Berserk Buff - Mines'),
(180383, 3870, NULL),
(180383, 6265, 'Battle for Gilneas - Food Buff - Waterworks 3'),
(180384, 3870, NULL),
(184964, 4568, 'EOTS - Speed Buff Blood Elf'),
(184965, 4568, 'EOTS - Restoration Buff Blood Elf'),
(184965, 5355, 'SOTA - Restoration Buff - West Workshop'),
(184966, 4568, 'EOTS - Berserk Buff Blood Elf'),
(184970, 4569, 'EOTS - Speed Buff Fel Reaver Ruins'),
(184971, 4569, 'EOTS - Restoration Buff Fel Reaver Ruins'),
(184971, 5356, 'SOTA - Restoration Buff - Courtyard'),
(184972, 4569, 'EOTS - Berserk Buff Fel Reaver Ruins'),
(184973, 4570, 'EOTS - Speed Buff Mage Tower'),
(184974, 4570, 'EOTS - Restoration Buff Mage Tower'),
(184975, 4570, 'EOTS - Berserk Buff Mage Tower'),
(184976, 4571, 'EOTS - Speed Buff Draenei Ruins'),
(184977, 4571, 'EOTS - Restoration Buff Draenei Ruins'),
(184977, 5354, 'SOTA - Restoration Buff - East Workshop'),
(184978, 4571, 'EOTS - Berserk Buff Draenei Ruins');

DELETE FROM `battleground_template_score`;
INSERT INTO `battleground_template_score` (`BattlegroundEntry`, `ScoreType`, `OrderIndex`) VALUES
(1, 11, 1),
(1, 12, 2),
(1, 13, 3),
(1, 14, 4),
(1, 15, 5),
(2, 7, 1),
(2, 8, 2),
(3, 9, 1),
(3, 10, 2),
(7, 7, 1),
(9, 16, 1),
(9, 17, 2),
(30, 9, 1),
(30, 10, 2),
(108, 7, 1),
(108, 8, 2),
(120, 9, 1),
(120, 10, 2);

UPDATE `battleground_template` SET `AllianceExploitLoc`=3705, `HordeExploitLoc`=3706, `MaxPoints`=1500, `ScriptName`='battleground_arathi_basin' WHERE `ID`=3;
UPDATE `battleground_template` SET `AllianceExploitLoc`=0, `HordeExploitLoc`=0, `Weight`=1, `MaxPoints`=3, `ScriptName`='battleground_twin_peaks' WHERE `ID`=108;
UPDATE `battleground_template` SET `AllianceExploitLoc`=3784, `HordeExploitLoc`=3785, `MaxPoints`=3, `ScriptName`='battleground_warsong_gulch' WHERE `ID`=2;
UPDATE `battleground_template` SET `AllianceExploitLoc`=0, `HordeExploitLoc`=0, `Weight`=1, `MaxPoints`=1500, `ScriptName`='battleground_battle_for_gilneas' WHERE `ID`=120;
UPDATE `battleground_template` SET `AllianceExploitLoc`=3986, `HordeExploitLoc`=3983, `ScriptName`='' WHERE `ID`=30; -- Isle of Conquest
UPDATE `battleground_template` SET `AllianceExploitLoc`=3664, `HordeExploitLoc`=3665, `ScriptName`='' WHERE `ID`=1; -- Alterac Valley
UPDATE `battleground_template` SET `AllianceExploitLoc`=3773, `HordeExploitLoc`=3772, `MaxPoints`=1500, `ScriptName`='battleground_eye_of_the_storm' WHERE `ID`=7;
UPDATE `battleground_template` SET `AllianceExploitLoc`=0, `HordeExploitLoc`=0, `ScriptName`='battleground_strand_of_the_ancients' WHERE `ID`=9;
DELETE FROM `battleground_template` WHERE `ID` IN (9/*Sota (removed in bfa)*/, 894);
INSERT INTO `battleground_template` (`ID`, `AllianceStartLoc`, `HordeStartLoc`, `StartMaxDist`, `Weight`, `AllianceExploitLoc`, `HordeExploitLoc`, `MaxPoints`, `ScriptName`, `Comment`) VALUES
(894, 6167, 6167, 0, 1, 0, 0, 1500, 'battleground_seething_shore', 'Seething Shore');

-- Warsong Gulch
SET @OGUID := 8000000;
SET @CGUID := 8000000;

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+1;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 13116, 489, 3277, 3321, 1, 0, 0, 0, 0, 1395.635, 1560.837, 329.869, 6.114909, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: Silverwing Hold - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+1, 13117, 489, 3277, 3321, 1, 0, 0, 0, 0, 1064.17, 1371.644, 328.8477, 2.635621, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- Horde Spirit Guide (Area: Silverwing Hold - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual; 22011 - Spirit Heal Channel)


DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+18;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
--  gates etc
(@OGUID+0, 180322, 489, 0, 0, 1, 0, 0, 1492.347, 1458.34, 342.3712, 6.248279, 0, 0, -0.01745224, 0.9998477, 7200, 255, 1, 26124), -- Ghost Gate
(@OGUID+1, 179920, 489, 0, 0, 1, 0, 0, 1468.507, 1494.361, 351.8624, 3.115388, 0, 0, 0.9999142, 0.01310196, 7200, 255, 1, 26124), -- Doodad_PortcullisActive03
(@OGUID+2, 179921, 489, 0, 0, 1, 0, 0, 1471.559, 1458.782, 362.6338, 3.115388, 0, 0, 0.9999142, 0.01310196, 7200, 255, 1, 26124), -- Doodad_PortcullisActive04
(@OGUID+3, 180322, 489, 0, 0, 1, 0, 0, 951.0469, 1422.752, 344.9273, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Ghost Gate
(@OGUID+4, 242273, 489, 0, 0, 1, 0, 0, 1524.5, 1514.366, 358.001, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Collision PC Size
(@OGUID+5, 179916, 489, 0, 0, 1, 0, 0, 949.1663, 1423.773, 345.6241, 5.707506, -0.01673365, -0.004956245, -0.2839718, 0.9586738, 7200, 255, 1, 26124), -- Doodad_RazorfenDoor01
(@OGUID+6, 179917, 489, 0, 0, 1, 0, 0, 953.0507, 1459.843, 340.6526, 4.28657, -0.1971817, 0.1575089, -0.8239479, 0.5073659, 7200, 255, 1, 26124), -- Doodad_RazorfenDoor02
(@OGUID+7, 179918, 489, 0, 0, 1, 0, 0, 1503.339, 1493.47, 352.1894, 3.115388, 0, 0, 0.9999142, 0.01310196, 7200, 255, 1, 26124), -- Doodad_PortcullisActive01
(@OGUID+8, 179919, 489, 0, 0, 1, 0, 0, 1492.482, 1457.916, 342.9695, 3.115388, 0, 0, 0.9999142, 0.01310196, 7200, 255, 1, 26124), -- Doodad_PortcullisActive02
(@OGUID+9, 180322, 489, 0, 0, 1, 0, 0, 1503.466, 1493.367, 351.7352, 6.248279, 0, 0, -0.01745224, 0.9998477, 7200, 255, 1, 26124), -- Ghost Gate
(@OGUID+10, 180322, 489, 0, 0, 1, 0, 0, 954.4479, 1459.583, 341.527, 0.05235888, 0, 0, 0.02617645, 0.9996573, 7200, 255, 1, 26124), -- Ghost Gate
--  Flags
(@OGUID+11, 227740, 489, 0, 0, 1, 0, 0, 916.507, 1433.826, 346.3796, 0.01745246, 0, 0, 0.00872612, 0.9999619, 7200, 255, 1, 26124), -- Horde Flag
(@OGUID+12, 227741, 489, 0, 0, 1, 0, 0, 1540.424, 1481.325, 351.8284, 3.089183, 0, 0, 0.9996567, 0.02620165, 7200, 255, 1, 26124), -- Alliance Flag
--  Buffs
(@OGUID+13, 179904, 489, 0, 0, 1, 0, 0, 1317.506, 1550.851, 313.2344, 6.021387, 0, 0, -0.1305256, 0.9914449, 7200, 255, 1, 26124), -- Food Buff
(@OGUID+14, 179906, 489, 0, 0, 1, 0, 0, 1110.451, 1353.656, 316.5181, 5.602507, 0, 0, -0.333807, 0.9426414, 7200, 255, 1, 26124), -- Food Buff
(@OGUID+15, 179905, 489, 0, 0, 1, 0, 0, 1320.094, 1378.79, 314.7532, 1.186823, 0, 0, 0.5591927, 0.8290377, 7200, 255, 1, 26124), -- Berserk Buff
(@OGUID+16, 179907, 489, 0, 0, 1, 0, 0, 1139.688, 1560.288, 306.8432, 3.839725, 0, 0, -0.9396925, 0.3420205, 7200, 255, 1, 26124), -- Berserk Buff
(@OGUID+17, 179899, 489, 0, 0, 1, 0, 0, 1005.105, 1448.015, 335.9118, 1.605702, 0, 0, 0.7193394, 0.6946588, 7200, 255, 1, 26124), -- Speed Buff
(@OGUID+18, 179871, 489, 0, 0, 1, 0, 0, 1449.93, 1470.71, 342.6346, 4.642576, 0, 0, -0.7313538, 0.6819983, 7200, 255, 1, 26124); -- Speed Buff

DELETE FROM `battleground_door` WHERE `battlegroundentry`=2;
INSERT INTO `battleground_door` (`BattlegroundEntry`, `GameObjectSpawnid`, `Comment`) VALUES
(2, @OGUID+0000, 'Warsong Gulch - Ghost Gate'),
(2, @OGUID+0001, 'Warsong Gulch - Portcullis Active 03'),
(2, @OGUID+0002, 'Warsong Gulch - Portcullis Active 04'),
(2, @OGUID+0003, 'Warsong Gulch - Ghost Gate'),
(2, @OGUID+0005, 'Warsong Gulch - Razorfen Door 01'),
(2, @OGUID+0006, 'Warsong Gulch - Razorfen Door 02'),
(2, @OGUID+0007, 'Warsong Gulch - Portcullis Active 01'),
(2, @OGUID+0008, 'Warsong Gulch - Portcullis Active 02'),
(2, @OGUID+0009, 'Warsong Gulch - Ghost Gate'),
(2, @OGUID+0010, 'Warsong Gulch - Ghost Gate');

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+18;
INSERT INTO `gameobject_addon` (`guid`, `parent_rotation0`, `parent_rotation1`, `parent_rotation2`, `parent_rotation3`) VALUES
(@OGUID+1, 0, 0, 0.6977905, 0.7163019), -- Doodad_PortcullisActive03
(@OGUID+2, 0, 0, 0.6977905, 0.7163019), -- Doodad_PortcullisActive04
(@OGUID+5, 0, 0, -0.7071068, 0.7071068), -- Doodad_RazorfenDoor01
(@OGUID+6, 0, 0, -0.7071068, 0.7071068), -- Doodad_RazorfenDoor02
(@OGUID+7, 0, 0, 0.6977905, 0.7163019), -- Doodad_PortcullisActive01
(@OGUID+8, 0, 0, 0.6977905, 0.7163019); -- Doodad_PortcullisActive02

-- Arathi Basin
DELETE FROM `gameobject_template` WHERE `entry` IN (227544 /*Capture Point*/, 227538 /*Capture Point*/, 227522 /*Capture Point*/, 227420 /*Capture Point*/, 227536 /*Capture Point*/);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `RequiredLevel`, `VerifiedBuild`) VALUES
(227544, 42, 21202, 'Capture Point', '', '', '', 1, 60000, 0, 1, 2178, 10225, 10227, 10229, 10224, 10226, 10228, 8805, 39387, 39388, 39389, 39390, 39391, 39392, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Capture Point
(227538, 42, 21202, 'Capture Point', '', '', '', 1, 60000, 0, 1, 2178, 10231, 10233, 10235, 10230, 10232, 10234, 8809, 39404, 39405, 39406, 39407, 39408, 39409, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Capture Point
(227522, 42, 21202, 'Capture Point', '', '', '', 1, 60000, 0, 1, 2178, 10212, 10214, 10216, 10211, 10213, 10215, 8799, 8876, 8773, 8770, 8874, 8769, 8774, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Capture Point
(227420, 42, 21202, 'Capture Point', '', '', '', 1, 60000, 0, 1, 2178, 10200, 10204, 10201, 10199, 10203, 10202, 5834, 28523, 28527, 28525, 28522, 28526, 28524, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Capture Point
(227536, 42, 21202, 'Capture Point', '', '', '', 1, 60000, 0, 1, 2178, 10218, 91869, 10222, 10217, 91868, 10221, 8808, 39398, 39399, 39400, 39401, 39402, 39403, 42975, 42976, 42978, 42979, 42980, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- Capture Point

UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=180256; -- HORDE DOOR
UPDATE `gameobject_template` SET `Data8`=0, `VerifiedBuild`=26124 WHERE `entry`=180255; -- ALLIANCE DOOR

SET @OGUID := 8001000;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+148;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 180322, 529, 0, 0, 1, 0, 0, 708.1077, 708.618, -17.95484, 0.7679439, 0, 0, 0.3746061, 0.927184, 7200, 255, 1, 26124), -- Ghost Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+3, 180256, 529, 0, 0, 1, 0, 0, 708.0903, 708.4479, -17.3899, 3.89209, -0.05029058, -0.01512623, -0.9292164, 0.3657854, 7200, 255, 1, 26124), -- HORDE DOOR (Area: -Unknown- - Difficulty: Normal)
(@OGUID+4, 180255, 529, 0, 0, 1, 0, 0, 1284.597, 1281.167, -15.97792, 0.7068579, 0.01295662, -0.06028748, 0.3449583, 0.9365903, 7200, 255, 1, 26124), -- ALLIANCE DOOR (Area: -Unknown- - Difficulty: Normal)
(@OGUID+5, 180322, 529, 0, 0, 1, 0, 0, 1284.636, 1280.682, -16.35919, 0.7155849, 0, 0, 0.3502073, 0.9366722, 7200, 255, 1, 26124), -- Ghost Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+9, 201893, 529, 0, 0, 1, 0, 0, 984.3621, 1007.395, -42.60554, 6.091198, 0, 0, -0.09584618, 0.9953961, 7200, 255, 1, 26124), -- Anvil (Area: -Unknown- - Difficulty: Normal)
(@OGUID+10, 201892, 529, 0, 0, 1, 0, 0, 990.9336, 1008.035, -44.41349, 2.373644, 0, 0, 0.9271832, 0.3746083, 7200, 255, 1, 26124), -- Forge (Area: -Unknown- - Difficulty: Normal)
(@OGUID+11, 201895, 529, 0, 0, 1, 0, 0, 996.198, 1001.877, -42.61397, 3.167798, 0, 0, -0.9999142, 0.01310196, 7200, 255, 1, 26124), -- Anvil (Area: -Unknown- - Difficulty: Normal)
(@OGUID+12, 201894, 529, 0, 0, 1, 0, 0, 991.4836, 1014.496, -42.60554, 4.607672, 0, 0, -0.743144, 0.6691315, 7200, 255, 1, 26124), -- Anvil (Area: -Unknown- - Difficulty: Normal)
(@OGUID+13, 201897, 529, 0, 0, 1, 0, 0, 824.0557, 842.1412, -56.60288, 0.3228863, 0, 0, 0.1607428, 0.9869964, 7200, 255, 1, 26124), -- Fireplace (Area: -Unknown- - Difficulty: Normal)
(@OGUID+14, 201896, 529, 0, 0, 1, 0, 0, 990.3508, 1001.441, -42.60554, 1.134463, 0, 0, 0.5372992, 0.8433917, 7200, 255, 1, 26124), -- Anvil (Area: -Unknown- - Difficulty: Normal)
(@OGUID+15, 201898, 529, 0, 0, 1, 0, 0, 824.0732, 843.0356, -56.60288, 1.614428, 0, 0, 0.7223635, 0.6915136, 7200, 255, 1, 26124), -- Fireplace (Area: -Unknown- - Difficulty: Normal)
(@OGUID+17, 227420, 529, 3358, 0, 1, 0, 0, 1166.856, 1200.122, -56.72062, 0.8860929, 0, 0, 0.4286938, 0.9034498, 7200, 255, 1, 26124), -- Capture Point (Area: 0 - Difficulty: 0)
(@OGUID+18, 227522, 529, 3358, 3420, 1, 0, 0, 977.5052, 1051.073, -44.79257, 0.497109, 0, 0, 0.2460032, 0.969269, 7200, 255, 1, 26124), -- Capture Point (Area: Farm - Difficulty: 0)
(@OGUID+19, 227538, 529, 3358, 3420, 1, 0, 0, 1146.929, 848.2274, -110.9176, 2.417101, 0, 0, 0.9351034, 0.354375, 7200, 255, 1, 26124), -- Capture Point (Area: Farm - Difficulty: 0)
(@OGUID+20, 227544, 529, 3358, 3420, 1, 0, 0, 856.8663, 1150.214, 11.38484, 0.9897078, 0, 0, 0.4749031, 0.8800381, 7200, 255, 1, 26124), -- Capture Point (Area: Farm - Difficulty: 0)
(@OGUID+21, 180224, 529, 3358, 3420, 1, 0, 0, 831.1494, 856.9456, -57.99989, 5.567601, 0, 0, -0.3502073, 0.9366722, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+22, 180220, 529, 3358, 3420, 1, 0, 0, 794.9638, 850.4819, -56.07512, 0.157079, 0, 0, 0.07845879, 0.9969174, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+23, 180220, 529, 3358, 3420, 1, 0, 0, 808.805, 797.4338, -57.97301, 2.635444, 0, 0, 0.9681473, 0.2503814, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+24, 180220, 529, 3358, 3420, 1, 0, 0, 821.9766, 813.5156, -58.03127, 3.543024, 0, 0, -0.9799242, 0.1993704, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+25, 180224, 529, 3358, 3420, 1, 0, 0, 825.284, 818.2079, -57.74417, 6.248279, 0, 0, -0.01745224, 0.9998477, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+26, 180226, 529, 3358, 3420, 1, 0, 0, 830.4949, 826.9015, -58.00945, 5.218536, 0, 0, -0.5075378, 0.8616294, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+27, 180220, 529, 3358, 3420, 1, 0, 0, 851.5027, 847.1608, -57.92309, 2.91469, 0, 0, 0.9935713, 0.1132084, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+28, 180223, 529, 3358, 3420, 1, 0, 0, 837.8289, 828.6371, -57.59694, 5.969027, 0, 0, -0.1564341, 0.9876884, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+29, 180226, 529, 3358, 3420, 1, 0, 0, 817.1022, 794.5945, -58.00546, 5.480334, 0, 0, -0.3907309, 0.920505, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+30, 180223, 529, 3358, 3420, 1, 0, 0, 855.9211, 843.1013, -57.85567, 5.550147, 0, 0, -0.3583679, 0.9335805, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+31, 180224, 529, 3358, 3420, 1, 0, 0, 838.4695, 869.584, -57.87056, 1.466076, 0, 0, 0.6691303, 0.743145, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+32, 180226, 529, 3358, 3420, 1, 0, 0, 819.5731, 873.3076, -58.26316, 6.021387, 0, 0, -0.1305256, 0.9914449, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+33, 180223, 529, 3358, 3420, 1, 0, 0, 811.6433, 800.1112, -57.95899, 2.722713, 0, 0, 0.9781475, 0.2079121, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+34, 180223, 529, 3358, 3420, 1, 0, 0, 846.7038, 822.0936, -58.05201, 0.1396245, 0, 0, 0.06975555, 0.9975641, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+35, 180227, 529, 3358, 3420, 1, 0, 0, 846.7749, 842.6201, -58.24146, 2.949595, 0, 0, 0.9953957, 0.09585124, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+36, 180225, 529, 3358, 3420, 1, 0, 0, 835.3809, 821.8143, -57.99535, 0.2967052, 0, 0, 0.147809, 0.9890159, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+37, 180224, 529, 3358, 3420, 1, 0, 0, 845.5385, 854.3147, -57.67622, 6.126106, 0, 0, -0.07845879, 0.9969174, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+38, 180227, 529, 3358, 3420, 1, 0, 0, 814.6383, 859.4977, -57.70895, 4.502952, 0, 0, -0.7771454, 0.6293211, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+39, 180226, 529, 3358, 3420, 1, 0, 0, 844.0472, 827.609, -57.83574, 2.164206, 0, 0, 0.882947, 0.4694727, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+40, 180220, 529, 3358, 3420, 1, 0, 0, 816.9979, 851.5518, -57.37224, 2.33874, 0, 0, 0.9205046, 0.3907318, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+41, 180223, 529, 3358, 3420, 1, 0, 0, 840.2105, 844.9096, -58.18686, 1.099556, 0, 0, 0.5224981, 0.8526405, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+42, 180222, 529, 3358, 3420, 1, 0, 0, 811.9866, 861.662, -57.81026, 1.308995, 0, 0, 0.6087608, 0.7933538, 7200, 255, 1, 26124), -- Arathi Basin Bone02 (Area: Farm - Difficulty: 0)
(@OGUID+43, 180220, 529, 3358, 3420, 1, 0, 0, 852.5516, 820.9903, -57.81715, 2.82743, 0, 0, 0.9876881, 0.1564362, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+44, 180225, 529, 3358, 3420, 1, 0, 0, 828.6318, 812.0804, -57.83467, 1.239183, 0, 0, 0.5807028, 0.8141156, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+45, 180224, 529, 3358, 3420, 1, 0, 0, 811.6783, 790.2651, -57.92487, 0.3490652, 0, 0, 0.1736479, 0.9848078, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+46, 180227, 529, 3358, 3420, 1, 0, 0, 834.0167, 839.4653, -58.28602, 3.874631, 0, 0, -0.9335804, 0.358368, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+47, 180220, 529, 3358, 3420, 1, 0, 0, 803.941, 826.6201, -57.41241, 2.059488, 0, 0, 0.8571672, 0.5150382, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+48, 180223, 529, 3358, 3420, 1, 0, 0, 845.4614, 882.7166, -58.51382, 3.036838, 0, 0, 0.9986286, 0.05235322, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+49, 180224, 529, 3358, 3420, 1, 0, 0, 822.1501, 864.6448, -57.79387, 0.2617982, 0, 0, 0.1305256, 0.9914449, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+50, 180227, 529, 3358, 3420, 1, 0, 0, 826.0211, 814.397, -57.85232, 0.5934101, 0, 0, 0.2923708, 0.956305, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+51, 180223, 529, 3358, 3420, 1, 0, 0, 825.9539, 823.0765, -57.81567, 5.602507, 0, 0, -0.333807, 0.9426414, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+52, 180224, 529, 3358, 3420, 1, 0, 0, 854.2534, 861.6038, -58.56574, 2.548179, 0, 0, 0.9563046, 0.2923723, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+53, 180227, 529, 3358, 3420, 1, 0, 0, 805.4304, 792.6251, -58.18983, 4.852017, 0, 0, -0.6560583, 0.7547102, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+54, 180223, 529, 3358, 3420, 1, 0, 0, 826.0455, 846.4521, -57.71627, 0.7853968, 0, 0, 0.3826828, 0.9238798, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+55, 180227, 529, 3358, 3420, 1, 0, 0, 857.9197, 853.4661, -58.55914, 4.799657, 0, 0, -0.6755896, 0.7372779, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+56, 180222, 529, 3358, 3420, 1, 0, 0, 857.8708, 828.8326, -57.88155, 1.797689, 0, 0, 0.782608, 0.6225148, 7200, 255, 1, 26124), -- Arathi Basin Bone02 (Area: Farm - Difficulty: 0)
(@OGUID+57, 180225, 529, 3358, 3420, 1, 0, 0, 809.782, 833.8421, -57.06508, 4.642576, 0, 0, -0.7313538, 0.6819983, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+58, 180224, 529, 3358, 3420, 1, 0, 0, 848.5848, 836.0681, -57.8643, 5.445428, 0, 0, -0.4067364, 0.9135455, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+59, 180227, 529, 3358, 3420, 1, 0, 0, 844.1527, 874.6531, -58.14783, 5.340709, 0, 0, -0.45399, 0.8910068, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+60, 180226, 529, 3358, 3420, 1, 0, 0, 844.2293, 873.4052, -58.01434, 0.7330382, 0, 0, 0.3583679, 0.9335805, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+61, 180220, 529, 3358, 3420, 1, 0, 0, 823.5795, 875.8678, -58.16789, 5.811947, 0, 0, -0.2334452, 0.97237, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+62, 180223, 529, 3358, 3420, 1, 0, 0, 802.7711, 841.938, -57.41446, 4.869471, 0, 0, -0.6494474, 0.7604064, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+63, 180223, 529, 3358, 3420, 1, 0, 0, 793.2523, 824.0829, -56.55127, 3.42085, 0, 0, -0.9902678, 0.1391754, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+64, 180222, 529, 3358, 3420, 1, 0, 0, 816.1795, 820.5785, -57.73499, 4.34587, 0, 0, -0.8241262, 0.5664061, 7200, 255, 1, 26124), -- Arathi Basin Bone02 (Area: Farm - Difficulty: 0)
(@OGUID+65, 180227, 529, 3358, 3420, 1, 0, 0, 860.246, 831.6154, -57.97754, 0.5934101, 0, 0, 0.2923708, 0.956305, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+66, 180220, 529, 3358, 3420, 1, 0, 0, 845.9161, 880.2176, -58.42031, 5.585054, 0, 0, -0.34202, 0.9396927, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+67, 180227, 529, 3358, 3420, 1, 0, 0, 837.2013, 862.3518, -57.46105, 2.18166, 0, 0, 0.8870106, 0.4617491, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+68, 180220, 529, 3358, 3420, 1, 0, 0, 825.4523, 875.5281, -58.1246, 3.944446, 0, 0, -0.9205046, 0.3907318, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+69, 180223, 529, 3358, 3420, 1, 0, 0, 839.844, 846.0521, -58.17279, 2.757613, 0, 0, 0.9816265, 0.1908124, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+84, 180225, 529, 3358, 3420, 1, 0, 0, 825.9539, 823.0765, -57.81567, 5.602507, 0, 0, -0.333807, 0.9426414, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+85, 180227, 529, 3358, 3420, 1, 0, 0, 826.0211, 814.397, -57.85232, 0.5934101, 0, 0, 0.2923708, 0.956305, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+86, 180226, 529, 3358, 3420, 1, 0, 0, 860.246, 831.6154, -57.97754, 0.5934101, 0, 0, 0.2923708, 0.956305, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+87, 180220, 529, 3358, 3420, 1, 0, 0, 825.4523, 875.5281, -58.1246, 3.944446, 0, 0, -0.9205046, 0.3907318, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+88, 180223, 529, 3358, 3420, 1, 0, 0, 846.7038, 822.0936, -58.05201, 0.1396245, 0, 0, 0.06975555, 0.9975641, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+89, 180222, 529, 3358, 3420, 1, 0, 0, 825.284, 818.2079, -57.74417, 6.248279, 0, 0, -0.01745224, 0.9998477, 7200, 255, 1, 26124), -- Arathi Basin Bone02 (Area: Farm - Difficulty: 0)
(@OGUID+90, 180227, 529, 3358, 3420, 1, 0, 0, 830.4949, 826.9015, -58.00945, 5.218536, 0, 0, -0.5075378, 0.8616294, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+91, 180223, 529, 3358, 3420, 1, 0, 0, 811.6433, 800.1112, -57.95899, 2.722713, 0, 0, 0.9781475, 0.2079121, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+92, 180227, 529, 3358, 3420, 1, 0, 0, 837.2013, 862.3518, -57.46105, 2.18166, 0, 0, 0.8870106, 0.4617491, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+93, 180223, 529, 3358, 3420, 1, 0, 0, 811.6783, 790.2651, -57.92487, 0.3490652, 0, 0, 0.1736479, 0.9848078, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+94, 180224, 529, 3358, 3420, 1, 0, 0, 822.1501, 864.6448, -57.79387, 0.2617982, 0, 0, 0.1305256, 0.9914449, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+95, 180225, 529, 3358, 3420, 1, 0, 0, 838.4695, 869.584, -57.87056, 1.466076, 0, 0, 0.6691303, 0.743145, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+96, 180224, 529, 3358, 3420, 1, 0, 0, 851.5027, 847.1608, -57.92309, 2.91469, 0, 0, 0.9935713, 0.1132084, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+97, 180226, 529, 3358, 3420, 1, 0, 0, 817.1022, 794.5945, -58.00546, 5.480334, 0, 0, -0.3907309, 0.920505, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+98, 180222, 529, 3358, 3420, 1, 0, 0, 835.3809, 821.8143, -57.99535, 0.2967052, 0, 0, 0.147809, 0.9890159, 7200, 255, 1, 26124), -- Arathi Basin Bone02 (Area: Farm - Difficulty: 0)
(@OGUID+99, 180225, 529, 3358, 3420, 1, 0, 0, 837.8289, 828.6371, -57.59694, 5.969027, 0, 0, -0.1564341, 0.9876884, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+100, 180227, 529, 3358, 3420, 1, 0, 0, 844.2293, 873.4052, -58.01434, 0.7330382, 0, 0, 0.3583679, 0.9335805, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+101, 180223, 529, 3358, 3420, 1, 0, 0, 803.941, 826.6201, -57.41241, 2.059488, 0, 0, 0.8571672, 0.5150382, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+102, 180225, 529, 3358, 3420, 1, 0, 0, 793.2523, 824.0829, -56.55127, 3.42085, 0, 0, -0.9902678, 0.1391754, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+103, 180227, 529, 3358, 3420, 1, 0, 0, 814.6383, 859.4977, -57.70895, 4.502952, 0, 0, -0.7771454, 0.6293211, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+104, 180225, 529, 3358, 3420, 1, 0, 0, 840.2105, 844.9096, -58.18686, 1.099556, 0, 0, 0.5224981, 0.8526405, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+105, 180225, 529, 3358, 3420, 1, 0, 0, 855.9211, 843.1013, -57.85567, 5.550147, 0, 0, -0.3583679, 0.9335805, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+106, 180224, 529, 3358, 3420, 1, 0, 0, 839.844, 846.0521, -58.17279, 2.757613, 0, 0, 0.9816265, 0.1908124, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+107, 180226, 529, 3358, 3420, 1, 0, 0, 805.4304, 792.6251, -58.18983, 4.852017, 0, 0, -0.6560583, 0.7547102, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+108, 180220, 529, 3358, 3420, 1, 0, 0, 816.1795, 820.5785, -57.73499, 4.34587, 0, 0, -0.8241262, 0.5664061, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+109, 180224, 529, 3358, 3420, 1, 0, 0, 816.9979, 851.5518, -57.37224, 2.33874, 0, 0, 0.9205046, 0.3907318, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+110, 180223, 529, 3358, 3420, 1, 0, 0, 811.9866, 861.662, -57.81026, 1.308995, 0, 0, 0.6087608, 0.7933538, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+111, 180224, 529, 3358, 3420, 1, 0, 0, 794.9638, 850.4819, -56.07512, 0.157079, 0, 0, 0.07845879, 0.9969174, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+112, 180227, 529, 3358, 3420, 1, 0, 0, 844.0472, 827.609, -57.83574, 2.164206, 0, 0, 0.882947, 0.4694727, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+113, 180220, 529, 3358, 3420, 1, 0, 0, 823.5795, 875.8678, -58.16789, 5.811947, 0, 0, -0.2334452, 0.97237, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+114, 180225, 529, 3358, 3420, 1, 0, 0, 857.8708, 828.8326, -57.88155, 1.797689, 0, 0, 0.782608, 0.6225148, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+115, 180227, 529, 3358, 3420, 1, 0, 0, 834.0167, 839.4653, -58.28602, 3.874631, 0, 0, -0.9335804, 0.358368, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+116, 180226, 529, 3358, 3420, 1, 0, 0, 846.7749, 842.6201, -58.24146, 2.949595, 0, 0, 0.9953957, 0.09585124, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+117, 180220, 529, 3358, 3420, 1, 0, 0, 828.6318, 812.0804, -57.83467, 1.239183, 0, 0, 0.5807028, 0.8141156, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+118, 180223, 529, 3358, 3420, 1, 0, 0, 831.1494, 856.9456, -57.99989, 5.567601, 0, 0, -0.3502073, 0.9366722, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+119, 180220, 529, 3358, 3420, 1, 0, 0, 852.5516, 820.9903, -57.81715, 2.82743, 0, 0, 0.9876881, 0.1564362, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+120, 180223, 529, 3358, 3420, 1, 0, 0, 845.5385, 854.3147, -57.67622, 6.126106, 0, 0, -0.07845879, 0.9969174, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+121, 180223, 529, 3358, 3420, 1, 0, 0, 802.7711, 841.938, -57.41446, 4.869471, 0, 0, -0.6494474, 0.7604064, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+122, 180223, 529, 3358, 3420, 1, 0, 0, 845.9161, 880.2176, -58.42031, 5.585054, 0, 0, -0.34202, 0.9396927, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+123, 180223, 529, 3358, 3420, 1, 0, 0, 808.805, 797.4338, -57.97301, 2.635444, 0, 0, 0.9681473, 0.2503814, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+124, 180225, 529, 3358, 3420, 1, 0, 0, 809.782, 833.8421, -57.06508, 4.642576, 0, 0, -0.7313538, 0.6819983, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+125, 180226, 529, 3358, 3420, 1, 0, 0, 857.9197, 853.4661, -58.55914, 4.799657, 0, 0, -0.6755896, 0.7372779, 7200, 255, 1, 26124), -- Arathi Basin Doomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+126, 180225, 529, 3358, 3420, 1, 0, 0, 848.5848, 836.0681, -57.8643, 5.445428, 0, 0, -0.4067364, 0.9135455, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+127, 180227, 529, 3358, 3420, 1, 0, 0, 844.1527, 874.6531, -58.14783, 5.340709, 0, 0, -0.45399, 0.8910068, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+128, 180225, 529, 3358, 3420, 1, 0, 0, 854.2534, 861.6038, -58.56574, 2.548179, 0, 0, 0.9563046, 0.2923723, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile03 (Area: Farm - Difficulty: 0)
(@OGUID+129, 180220, 529, 3358, 3420, 1, 0, 0, 826.0455, 846.4521, -57.71627, 0.7853968, 0, 0, 0.3826828, 0.9238798, 7200, 255, 1, 26124), -- Arathi Basin Bone01 (Area: Farm - Difficulty: 0)
(@OGUID+130, 180224, 529, 3358, 3420, 1, 0, 0, 845.4614, 882.7166, -58.51382, 3.036838, 0, 0, 0.9986286, 0.05235322, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsBonePile02 (Area: Farm - Difficulty: 0)
(@OGUID+131, 180227, 529, 3358, 3420, 1, 0, 0, 819.5731, 873.3076, -58.26316, 6.021387, 0, 0, -0.1305256, 0.9914449, 7200, 255, 1, 26124), -- Arathi Basin Gloomweed01 (Area: Farm - Difficulty: 0)
(@OGUID+132, 180223, 529, 3358, 3420, 1, 0, 0, 821.9766, 813.5156, -58.03127, 3.543024, 0, 0, -0.9799242, 0.1993704, 7200, 255, 1, 26124), -- Arathi Basin BlastedLandsSkull01 (Area: Farm - Difficulty: 0)
(@OGUID+133, 227536, 529, 0, 0, 1, 0, 0, 806.25, 874.2795, -56.01028, 0.8182427, 0, 0, 0.3978033, 0.9174708, 7200, 255, 1, 26124);

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+148;
INSERT INTO `gameobject_addon` (`guid`, `parent_rotation0`, `parent_rotation1`, `parent_rotation2`, `parent_rotation3`) VALUES
(@OGUID+9, 0, 0, 0.9271838, 0.3746068), -- Anvil
(@OGUID+10, 0, 0, 0.9271838, 0.3746068), -- Forge
(@OGUID+11, 0, 0, 0.9271838, 0.3746068), -- Anvil
(@OGUID+12, 0, 0, 0.9271838, 0.3746068), -- Anvil
(@OGUID+13, 0, 0, -0.685183, 0.728371), -- Fireplace
(@OGUID+14, 0, 0, 0.9271838, 0.3746068), -- Anvil
(@OGUID+15, 0, 0, -0.685183, 0.728371); -- Fireplace

DELETE FROM `battleground_door` WHERE `BattlegroundEntry`=3;
INSERT INTO `battleground_door` (`BattlegroundEntry`, `GameObjectSpawnid`, `Comment`) VALUES
(3, @OGUID+1000, 'Arathi Basin - Ghost Gate'),
(3, @OGUID+1003, 'Arathi Basin - Horde Door'),
(3, @OGUID+1004, 'Arathi Basin - Alliance Door'),
(3, @OGUID+1005, 'Arathi Basin - Ghost Gate');

SET @CGUID := 8001000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+11;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 13116, 529, 0, 0, 1, 0, 0, 0, 0, 1347.453, 1274.054, -11.36945, 5.742133, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+1, 13117, 529, 0, 0, 1, 0, 0, 0, 0, 713.0104, 645.0382, -10.70946, 4.852015, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
-- Stables
(@CGUID+2, 13116, 529, 0, 0, 1, 0, 0, 0, 0, 1199.666, 1168.635, -56.32924, 5.201081, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: Gold Mine - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+3, 13117, 529, 0, 0, 1, 0, 0, 0, 0, 1199.666, 1168.635, -56.32924, 5.201081, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Spirit Guide (Area: Gold Mine - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel) -- TODO SNIFF
-- Blacksmith
(@CGUID+4, 13117, 529, 0, 0, 1, 0, 0, 0, 0, 1018.021, 958.7031, -42.74471, 2.298979, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Spirit Guide (Area: Gold Mine - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+5, 13116, 529, 0, 0, 1, 0, 0, 0, 0, 1015.828, 961.3568, -42.94895, 4.991642, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
-- Farm
(@CGUID+6, 13117, 529, 0, 0, 1, 0, 0, 0, 0, 833.662, 790.6814, -57.14627, 4.660029, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+7, 13116, 529, 0, 0, 1, 0, 0, 0, 0, 833.662, 790.6814, -57.14627, 4.660029, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel) -- TODO SNIFF
-- Gold Mine
(@CGUID+8, 13117, 529, 0, 0, 1, 0, 0, 0, 0, 1204.323, 786.4811, -83.18834, 5.67232, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Spirit Guide (Area: Lumber Mill - Difficulty: 0) (Auras: 9036 - Ghost)
(@CGUID+9, 13116, 529, 0, 0, 1, 0, 0, 0, 0, 1204.323, 786.4811, -83.18834, 5.67232, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: Lumber Mill - Difficulty: 0) (Auras: 9036 - Ghost) - TODO SNIFF
-- Lumbermill
(@CGUID+10, 13116, 529, 0, 0, 1, 0, 0, 0, 0, 775.1235, 1206.052, 15.88072, 2.146755, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Spirit Guide (Area: Gold Mine - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+11, 13117, 529, 0, 0, 1, 0, 0, 0, 0, 775.2444, 1206.366, 15.88072, 2.007129, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- Horde Spirit Guide (Area: Blacksmith - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)

DELETE FROM `battleground_capture_point_creature` WHERE `BattlegroundEntry`=3;
INSERT INTO `battleground_capture_point_creature` (`BattlegroundEntry`, `CapturePointKey`, `SpawnId`, `TeamId`, `Comment`) VALUES
(3, 227420, @CGUID+2, 0, 'Alliance Spirit Guide Stables'),
(3, 227420, @CGUID+3, 1, 'TODO SNIFF - Horde Spirit Guide Stables'),
(3, 227522, @CGUID+4, 1, 'Horde Spirit Guide Blacksmith'),
(3, 227522, @CGUID+5, 0, 'Alliance Spirit Guide Blacksmith'),
(3, 227536, @CGUID+6, 1, 'Horde Spirit Guide Farm'),
(3, 227536, @CGUID+7, 0, 'TODO SNIFF - Alliance Spirit Guide Farm'),
(3, 227538, @CGUID+8, 1, 'Horde Spirit Guide Gold Mine'),
(3, 227538, @CGUID+9, 0, 'Alliance Spirit Guide Gold Mine'),
(3, 227544, @CGUID+10, 0, 'Alliance Spirit Guide Lumber Mill'),
(3, 227544, @CGUID+11, 1, 'Horde Spirit Guide Lumber Mill');



DELETE FROM `creature_template_addon` WHERE `entry` IN (20212, 20213);
INSERT INTO `creature_template_addon` (`entry`, `bytes2`, `auras`) VALUES
(20212, 1, '66157 8149'),
(20213, 1, '66157 8149');

-- Eye of the Storm
SET @CGUID := 8003000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+24;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 15384, 566, 0, 0, 1, 0, 0, 0, 0, 2523.898, 1596.936, 1262.958, 0.4886922, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- OLDWorld Trigger (DO NOT DELETE) (Area: -Unknown- - Difficulty: Normal)
(@CGUID+1, 15384, 566, 0, 0, 1, 0, 0, 0, 0, 2249.008, 1531.874, 1500.995, 5.445427, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- OLDWorld Trigger (DO NOT DELETE) (Area: -Unknown- - Difficulty: Normal) (Auras: )
(@CGUID+2, 15384, 566, 0, 0, 1, 0, 0, 0, 0, 1807.92, 1539.641, 1259.719, 4.433136, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- OLDWorld Trigger (DO NOT DELETE) (Area: -Unknown- - Difficulty: Normal)
(@CGUID+7, 13117, 566, 0, 0, 1, 0, 0, 0, 0, 1805.552, 1539.557, 1248.293, 0.08726646, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Fel Reaver Ruins - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+8, 13116, 566, 0, 0, 1, 0, 0, 0, 0, 2526.273, 1596.498, 1248.696, 3.106686, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Eye of the Storm - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
-- ??
(@CGUID+9, 13116, 566, 0, 0, 1, 0, 0, 0, 0, 2357.604, 1457.805, 1185.418, 3.455752, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Mage Tower - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+10, 13117, 566, 0, 0, 1, 0, 0, 0, 0, 2357.619, 1457.821, 1185.418, 3.630285, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Fel Reaver Ruins - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
-- ??
(@CGUID+11, 13117, 566, 0, 0, 1, 0, 0, 0, 0, 2007.931, 1673.318, 1182.207, 0.5585054, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Eye of the Storm - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual)
(@CGUID+12, 13116, 566, 0, 0, 1, 0, 0, 0, 0, 2008.07, 1673.313, 1182.207, 0.5235988, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Fel Reaver Ruins - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
-- ??
(@CGUID+13, 13117, 566, 0, 0, 1, 0, 0, 0, 0, 2008.588, 1458.136, 1172.3, 5.707227, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Fel Reaver Ruins - Difficulty: 0) (Auras: 9036 - Ghost)
(@CGUID+14, 13116, 566, 0, 0, 1, 0, 0, 0, 0, 2008.465, 1458.08, 1172.302, 5.654867, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Mage Tower - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
-- ??
(@CGUID+15, 13116, 566, 0, 0, 1, 0, 0, 0, 0, 2358.497, 1680.34, 1173.237, 2.076942, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Eye of the Storm - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+16, 13117, 566, 0, 0, 1, 0, 0, 0, 0, 2358.762, 1680.406, 1173.237, 2.373648, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual; 22011 - Spirit Heal Channel)
-- honorable defenders
(@CGUID+17, 20212, 566, 0, 0, 1, 0, 0, 0, 0, 2044.28, 1729.68, 1189.96, 0.017453, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+18, 20212, 566, 0, 0, 1, 0, 0, 0, 0, 2048.83, 1393.65, 1194.49, 0.20944, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+19, 20212, 566, 0, 0, 1, 0, 0, 0, 0, 2286.56, 1402.36, 1197.11, 3.72381, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+20, 20212, 566, 0, 0, 1, 0, 0, 0, 0, 2284.48, 1731.23, 1189.99, 2.89725, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+21, 20213, 566, 0, 0, 1, 0, 0, 0, 0, 2044.28, 1729.68, 1189.96, 0.017453, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+22, 20213, 566, 0, 0, 1, 0, 0, 0, 0, 2048.83, 1393.65, 1194.49, 0.20944, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+23, 20213, 566, 0, 0, 1, 0, 0, 0, 0, 2286.56, 1402.36, 1197.11, 3.72381, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Honorable Defender
(@CGUID+24, 20213, 566, 0, 0, 1, 0, 0, 0, 0, 2284.48, 1731.23, 1189.99, 2.89725, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365); -- Honorable Defender

SET @OGUID := 8003000;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+92;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+1, 245854, 566, 0, 0, 1, 0, 0, 2279.986, 1770.158, 1199.252, 3.198537, 0, 0, -0.9995947, 0.02846831, 7200, 255, 1, 26365), -- Large Collision Wall (Area: -Unknown- - Difficulty: Normal)
(@OGUID+2, 184720, 566, 0, 0, 1, 0, 0, 1803.207, 1539.486, 1238.454, 3.138983, 0.1736479, 0, 0.984807, 0.001244878, 7200, 255, 1, 26365), -- Forcefield 001 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+3, 184897, 566, 0, 0, 1, 0, 0, 2266.252, 1758.492, 1215.417, 0.3141584, 0, 0, 0.1564341, 0.9876884, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair27 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+4, 184896, 566, 0, 0, 1, 0, 0, 2273.669, 1776.753, 1215.417, 5.209809, 0, 0, -0.5112925, 0.8594068, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair24 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+6, 184899, 566, 0, 0, 1, 0, 0, 2273.816, 1748.205, 1215.417, 1.099556, 0, 0, 0.5224981, 0.8526405, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair29 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+7, 245854, 566, 0, 0, 1, 0, 0, 2280.153, 1762.427, 1200.984, 1.823414, 0, 0, 0.7905502, 0.6123972, 7200, 255, 1, 26365), -- Large Collision Wall (Area: -Unknown- - Difficulty: Normal)
(@OGUID+8, 184898, 566, 0, 0, 1, 0, 0, 2268.176, 1753.696, 1215.417, 0.497418, 0, 0, 0.2461529, 0.969231, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair28 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+9, 184901, 566, 0, 0, 1, 0, 0, 2286.438, 1746.279, 1215.417, 1.884956, 0, 0, 0.8090172, 0.5877849, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair31 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+10, 184900, 566, 0, 0, 1, 0, 0, 2278.567, 1746.174, 1215.417, 1.282817, 0, 0, 0.5983248, 0.8012537, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair30 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+11, 184903, 566, 0, 0, 1, 0, 0, 2296.724, 1753.842, 1215.417, 2.670348, 0, 0, 0.9723692, 0.2334484, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair33 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+12, 184902, 566, 0, 0, 1, 0, 0, 2291.234, 1748.203, 1215.417, 2.068215, 0, 0, 0.8594065, 0.5112929, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair32 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+13, 184904, 566, 0, 0, 1, 0, 0, 2298.756, 1758.594, 1215.417, 2.853604, 0, 0, 0.9896507, 0.1434972, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair34 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+15, 266832, 566, 0, 0, 1, 0, 0, 2283.995, 1738.354, 1196.401, 4.799515, 0, 0, -0.675642, 0.7372299, 7200, 255, 1, 26365), -- Collision PC Size (Area: -Unknown- - Difficulty: Normal)
(@OGUID+17, 184881, 566, 0, 0, 1, 0, 0, 2265.006, 1775.977, 1224.293, 5.567603, 0, 0, -0.3502064, 0.9366726, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair09 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+18, 184880, 566, 0, 0, 1, 0, 0, 2266.262, 1777.337, 1224.293, 5.567603, 0, 0, -0.3502064, 0.9366726, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair08 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+19, 184883, 566, 0, 0, 1, 0, 0, 2264.011, 1762.015, 1224.245, 0.05235888, 0, 0, 0.02617645, 0.9996573, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair11 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+20, 184882, 566, 0, 0, 1, 0, 0, 2300.441, 1765.92, 1224.239, 3.996807, 0, 0, -0.9099607, 0.4146944, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair10 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+21, 184885, 566, 0, 0, 1, 0, 0, 2278.969, 1780.7, 1224.274, 4.878196, 0, 0, -0.6461239, 0.7632326, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair13 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+22, 184884, 566, 0, 0, 1, 0, 0, 2264.053, 1760.163, 1224.245, 0.05235888, 0, 0, 0.02617645, 0.9996573, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair12 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+23, 184719, 566, 0, 0, 1, 0, 0, 2527.597, 1596.906, 1238.454, 3.15914, 0.1736417, 0.001514435, -0.9847698, 0.008638578, 7200, 255, 1, 26365), -- Forcefield 000 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+24, 184887, 566, 0, 0, 1, 0, 0, 2279.094, 1754.725, 1224.172, 2.740162, 0, 0, 0.9799242, 0.1993704, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair15 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+25, 184886, 566, 0, 0, 1, 0, 0, 2282.662, 1781.071, 1224.274, 4.625124, 0, 0, -0.737277, 0.6755905, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair14 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+26, 184889, 566, 0, 0, 1, 0, 0, 2278.659, 1770.807, 1224.172, 3.57793, 0, 0, -0.9762955, 0.2164421, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair17 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+27, 184888, 566, 0, 0, 1, 0, 0, 2276.123, 1759.018, 1224.14, 3.57793, 0, 0, -0.9762955, 0.2164421, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair16 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+28, 184891, 566, 0, 0, 1, 0, 0, 2298.65, 1766.464, 1215.417, 3.455756, 0, 0, -0.9876881, 0.1564362, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair19 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+29, 184890, 566, 0, 0, 1, 0, 0, 2277.238, 1764.29, 1224.127, 2.949595, 0, 0, 0.9953957, 0.09585124, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair18 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+30, 184893, 566, 0, 0, 1, 0, 0, 2291.087, 1776.751, 1215.417, 4.241154, 0, 0, -0.8526392, 0.5225001, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair21 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+31, 184892, 566, 0, 0, 1, 0, 0, 2296.727, 1771.26, 1215.417, 3.639014, 0, 0, -0.9692307, 0.2461543, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair20 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+32, 184895, 566, 0, 0, 1, 0, 0, 2278.465, 1778.677, 1215.417, 5.026549, 0, 0, -0.5877848, 0.8090174, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair23 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+33, 184894, 566, 0, 0, 1, 0, 0, 2286.335, 1778.782, 1215.417, 4.424412, 0, 0, -0.8012533, 0.5983253, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair22 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+34, 184873, 566, 0, 0, 1, 0, 0, 2283.713, 1749.252, 1200.119, 1.745327, 0, 0, 0.7660437, 0.6427886, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair01 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+35, 266830, 566, 0, 0, 1, 0, 0, 2050.345, 1372.099, 1203.834, 0, 0, 0, 0, 1, 7200, 255, 1, 26365), -- Collision PC Size (Area: -Unknown- - Difficulty: Normal)
(@OGUID+36, 184875, 566, 0, 0, 1, 0, 0, 2297.311, 1778.667, 1224.293, 3.996807, 0, 0, -0.9099607, 0.4146944, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair03 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+37, 184874, 566, 0, 0, 1, 0, 0, 2295.95, 1779.923, 1224.293, 3.996807, 0, 0, -0.9099607, 0.4146944, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair02 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+38, 184877, 566, 0, 0, 1, 0, 0, 2268.952, 1745.033, 1224.293, 0.8552105, 0, 0, 0.4146929, 0.9099615, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair05 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+39, 184876, 566, 0, 0, 1, 0, 0, 2267.592, 1746.289, 1224.293, 0.8552105, 0, 0, 0.4146929, 0.9099615, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair04 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+40, 184879, 566, 0, 0, 1, 0, 0, 2299.896, 1748.979, 1224.293, 2.426008, 0, 0, 0.9366722, 0.3502074, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair07 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+41, 184878, 566, 0, 0, 1, 0, 0, 2298.64, 1747.619, 1224.293, 2.426008, 0, 0, 0.9366722, 0.3502074, 7200, 255, 1, 26365), -- Doodad_GeneralMedChair06 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+54, 208977, 566, 0, 0, 1, 0, 0, 2174.445, 1569.422, 1159.853, 4.625124, 0, 0, -0.737277, 0.6755905, 7200, 255, 1, 26365), -- Eye of the Storm Flag (Area: Eye of the Storm - Difficulty: 0)

(@OGUID+55, 184380, 566, 0, 0, 1, 0, 0, 2025.094, 1386.122, 1192.654, 2.373644, 0, 0, 0.9271832, 0.3746083, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+56, 184380, 566, 0, 0, 1, 0, 0, 2074.306, 1385.773, 1194.469, 0.4712385, 0, 0, 0.2334452, 0.97237, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+57, 184380, 566, 0, 0, 1, 0, 0, 2047.198, 1349.188, 1188.565, 4.625124, 0, 0, -0.737277, 0.6755905, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+59, 184380, 566, 0, 0, 1, 0, 0, 2092.354, 1775.458, 1187.079, 5.881761, 0, 0, -0.1993675, 0.9799248, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+60, 184380, 566, 0, 0, 1, 0, 0, 2057.457, 1735.075, 1187.906, 5.358162, 0, 0, -0.4461975, 0.8949345, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+61, 184380, 566, 0, 0, 1, 0, 0, 2032.252, 1729.533, 1190.325, 1.867502, 0, 0, 0.8038568, 0.5948228, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+62, 184380, 566, 0, 0, 1, 0, 0, 2270.87, 1784.099, 1186.438, 2.356195, 0, 0, 0.9238796, 0.3826832, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+63, 184380, 566, 0, 0, 1, 0, 0, 2268.597, 1737.019, 1186.754, 0.9424766, 0, 0, 0.45399, 0.8910068, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+65, 184380, 566, 0, 0, 1, 0, 0, 2301.019, 1741.493, 1187.49, 5.375615, 0, 0, -0.4383707, 0.8987942, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+66, 184380, 566, 0, 0, 1, 0, 0, 2245.382, 1366.455, 1195.182, 2.373644, 0, 0, 0.9271832, 0.3746083, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+67, 184380, 566, 0, 0, 1, 0, 0, 2276.799, 1400.441, 1196.22, 2.495818, 0, 0, 0.9483232, 0.3173059, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+68, 184380, 566, 0, 0, 1, 0, 0, 2305.764, 1404.597, 1199.333, 1.640607, 0, 0, 0.7313528, 0.6819993, 7200, 255, 1, 26365), -- Visual Banner (Horde) (Area: Fel Reaver Ruins - Difficulty: 0)

(@OGUID+69, 184381, 566, 0, 0, 1, 0, 0, 2276.797, 1400.408, 1196.334, 2.44346, 0, 0, 0.9396925, 0.3420205, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+70, 184381, 566, 0, 0, 1, 0, 0, 2245.396, 1366.413, 1195.279, 2.216565, 0, 0, 0.8949337, 0.4461992, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+71, 184381, 566, 0, 0, 1, 0, 0, 2305.776, 1404.557, 1199.385, 1.745327, 0, 0, 0.7660437, 0.6427886, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+72, 184381, 566, 0, 0, 1, 0, 0, 2270.836, 1784.08, 1186.757, 2.426008, 0, 0, 0.9366722, 0.3502074, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+73, 184381, 566, 0, 0, 1, 0, 0, 2300.856, 1741.247, 1187.793, 5.497789, 0, 0, -0.3826828, 0.9238798, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+74, 184381, 566, 0, 0, 1, 0, 0, 2269.127, 1737.703, 1186.815, 0.9948372, 0, 0, 0.4771585, 0.8788173, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+75, 184381, 566, 0, 0, 1, 0, 0, 2025.125, 1386.123, 1192.735, 2.391098, 0, 0, 0.9304171, 0.3665025, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Mage Tower - Difficulty: 0)
(@OGUID+76, 184381, 566, 0, 0, 1, 0, 0, 2047.191, 1349.193, 1189.003, 4.66003, 0, 0, -0.7253742, 0.6883547, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Mage Tower - Difficulty: 0)
(@OGUID+77, 184381, 566, 0, 0, 1, 0, 0, 2074.32, 1385.78, 1194.72, 0.4886912, 0, 0, 0.2419214, 0.9702958, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Mage Tower - Difficulty: 0)
(@OGUID+78, 184381, 566, 0, 0, 1, 0, 0, 2032.248, 1729.547, 1191.23, 1.797689, 0, 0, 0.782608, 0.6225148, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+79, 184381, 566, 0, 0, 1, 0, 0, 2092.339, 1775.474, 1187.504, 5.811947, 0, 0, -0.2334452, 0.97237, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Fel Reaver Ruins - Difficulty: 0)
(@OGUID+80, 184381, 566, 0, 0, 1, 0, 0, 2057.473, 1735.109, 1188.066, 5.305802, 0, 0, -0.469471, 0.8829479, 7200, 255, 1, 26365), -- Visual Banner (Alliance) (Area: Fel Reaver Ruins - Difficulty: 0)

(@OGUID+81, 184382, 566, 0, 0, 1, 0, 0, 2300.993, 1741.55, 1187.107, 5.375615, 0, 0, -0.4383707, 0.8987942, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+82, 184382, 566, 0, 0, 1, 0, 0, 2074.321, 1385.762, 1194.362, 0.4886912, 0, 0, 0.2419214, 0.9702958, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+83, 184382, 566, 0, 0, 1, 0, 0, 2270.84, 1784.12, 1186.147, 2.303831, 0, 0, 0.9135447, 0.4067384, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+84, 184382, 566, 0, 0, 1, 0, 0, 2047.188, 1349.194, 1188.573, 4.642576, 0, 0, -0.7313538, 0.6819983, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+85, 184382, 566, 0, 0, 1, 0, 0, 2245.41, 1366.441, 1195.111, 2.356195, 0, 0, 0.9238796, 0.3826832, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+86, 184382, 566, 0, 0, 1, 0, 0, 2276.833, 1400.438, 1196.147, 2.478367, 0, 0, 0.9455185, 0.3255684, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+87, 184382, 566, 0, 0, 1, 0, 0, 2032.257, 1729.557, 1191.08, 1.797689, 0, 0, 0.782608, 0.6225148, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+88, 184382, 566, 0, 0, 1, 0, 0, 2092.396, 1775.451, 1186.966, 5.899214, 0, 0, -0.1908083, 0.9816273, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+89, 184382, 566, 0, 0, 1, 0, 0, 2305.778, 1404.536, 1199.246, 1.570796, 0, 0, 0.7071066, 0.7071069, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+90, 184382, 566, 0, 0, 1, 0, 0, 2057.493, 1735.111, 1187.676, 5.340709, 0, 0, -0.45399, 0.8910068, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+91, 184382, 566, 0, 0, 1, 0, 0, 2268.465, 1736.839, 1186.743, 0.9424766, 0, 0, 0.45399, 0.8910068, 7200, 255, 1, 26365), -- Visual Banner (Neutral) (Area: Eye of the Storm - Difficulty: 0)
(@OGUID+92, 184382, 566, 0, 0, 1, 0, 0, 2025.137, 1386.134, 1192.548, 2.391098, 0, 0, 0.9304171, 0.3665025, 7200, 255, 1, 26365); -- Visual Banner (Neutral) (Area: Mage Tower - Difficulty: 0)


DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+87;
INSERT INTO `gameobject_addon` (`guid`, `parent_rotation0`, `parent_rotation1`, `parent_rotation2`, `parent_rotation3`) VALUES
(@OGUID+3, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair27
(@OGUID+4, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair24
(@OGUID+6, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair29
(@OGUID+8, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair28
(@OGUID+9, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair31
(@OGUID+10, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair30
(@OGUID+11, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair33
(@OGUID+12, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair32
(@OGUID+13, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair34
(@OGUID+17, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair09
(@OGUID+18, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair08
(@OGUID+19, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair11
(@OGUID+20, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair10
(@OGUID+21, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair13
(@OGUID+22, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair12
(@OGUID+24, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair15
(@OGUID+25, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair14
(@OGUID+26, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair17
(@OGUID+27, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair16
(@OGUID+28, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair19
(@OGUID+29, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair18
(@OGUID+30, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair21
(@OGUID+31, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair20
(@OGUID+32, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair23
(@OGUID+33, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair22
(@OGUID+34, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair01
(@OGUID+36, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair03
(@OGUID+37, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair02
(@OGUID+38, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair05
(@OGUID+39, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair04
(@OGUID+40, 0, 0, -0.6788007, 0.7343225), -- Doodad_GeneralMedChair07
(@OGUID+41, 0, 0, -0.6788007, 0.7343225); -- Doodad_GeneralMedChair06

DELETE FROM `battleground_door` WHERE `BattlegroundEntry`=7;
INSERT INTO `battleground_door` (`BattlegroundEntry`, `GameObjectSpawnid`, `Comment`) VALUES
(7, @OGUID+3002, 'Eye of the Storm - Force Field 001'),
(7, @OGUID+3023, 'Eye of the Storm - Force Field 000');

DELETE FROM `battleground_capture_point_gameobject` WHERE `BattlegroundEntry`=7;
INSERT INTO `battleground_capture_point_gameobject` (`BattlegroundEntry`, `CapturePointKey`, `SpawnId`, `TeamId`, `Comment`) VALUES
(7, 1, @OGUID+59, 1, NULL),
(7, 1, @OGUID+60, 1, NULL),
(7, 1, @OGUID+61, 1, NULL),
(7, 1, @OGUID+78, 0, NULL),
(7, 1, @OGUID+79, 0, NULL),
(7, 1, @OGUID+80, 0, NULL),
(7, 1, @OGUID+87, 2, NULL),
(7, 1, @OGUID+88, 2, NULL),
(7, 1, @OGUID+90, 2, NULL),
(7, 2, @OGUID+55, 1, NULL),
(7, 2, @OGUID+56, 1, NULL),
(7, 2, @OGUID+57, 1, NULL),
(7, 2, @OGUID+75, 0, NULL),
(7, 2, @OGUID+76, 0, NULL),
(7, 2, @OGUID+77, 0, NULL),
(7, 2, @OGUID+82, 2, NULL),
(7, 2, @OGUID+84, 2, NULL),
(7, 2, @OGUID+92, 2, NULL),
(7, 3, @OGUID+66, 1, NULL),
(7, 3, @OGUID+67, 1, NULL),
(7, 3, @OGUID+68, 1, NULL),
(7, 3, @OGUID+69, 0, NULL),
(7, 3, @OGUID+70, 0, NULL),
(7, 3, @OGUID+71, 0, NULL),
(7, 3, @OGUID+85, 2, NULL),
(7, 3, @OGUID+86, 2, NULL),
(7, 3, @OGUID+89, 2, NULL),
(7, 4, @OGUID+62, 1, NULL),
(7, 4, @OGUID+63, 1, NULL),
(7, 4, @OGUID+65, 1, NULL),
(7, 4, @OGUID+72, 0, NULL),
(7, 4, @OGUID+73, 0, NULL),
(7, 4, @OGUID+74, 0, NULL),
(7, 4, @OGUID+81, 2, NULL),
(7, 4, @OGUID+83, 2, NULL),
(7, 4, @OGUID+91, 2, NULL);

DELETE FROM `battleground_capture_point_creature` WHERE `BattlegroundEntry`=7;
INSERT INTO `battleground_capture_point_creature` (`BattlegroundEntry`, `CapturePointKey`, `SpawnId`, `TeamId`, `Comment`) VALUES
(7, 1, @OGUID+11, 1, NULL),
(7, 1, @OGUID+12, 0, NULL),
(7, 1, @OGUID+17, 1, NULL),
(7, 1, @OGUID+21, 0, NULL),
(7, 2, @OGUID+13, 1, NULL),
(7, 2, @OGUID+14, 0, NULL),
(7, 2, @OGUID+18, 1, NULL),
(7, 2, @OGUID+22, 0, NULL),
(7, 3, @OGUID+09, 0, NULL),
(7, 3, @OGUID+10, 1, NULL),
(7, 3, @OGUID+19, 1, NULL),
(7, 3, @OGUID+23, 0, NULL),
(7, 4, @OGUID+15, 0, NULL),
(7, 4, @OGUID+16, 1, NULL),
(7, 4, @OGUID+20, 1, NULL),
(7, 4, @OGUID+24, 0, NULL);




UPDATE `gameobject_template_addon` SET `faction`=2059, `flags`=32 WHERE `entry`=208977; -- Netherstorm Flag
UPDATE `gameobject_template` SET `type`=36, `name`='Eye of the Storm Flag', `Data0`=2178, `Data1`=34976, `Data3`=1, `Data5`=1, `Data6`=1, `Data7`=10000, `Data8`=20000, `Data9`=228508, `Data11`=8863, `VerifiedBuild`=26365 WHERE `entry`=208977; -- Netherstorm Flag

DELETE FROM `gameobject_template` WHERE `entry`=228508;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `RequiredLevel`, `VerifiedBuild`) VALUES
(228508, 37, 7153, 'Eye of the Storm Flag', '', '', '', 2.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26365); -- Eye of the Storm Flag


-- Twin Peaks
SET @CGUID := 8004000;
DELETE FROM `creature` WHERE (`guid` BETWEEN @CGUID+0 AND @CGUID+175) OR `map`=726;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+52, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1750.692, 430.4157, -8.06117, 0.4980546, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+53, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1773.512, 218.0191, -14.04847, 2.879793, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+54, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1932.408, 202.8203, -16.89513, 4.967034, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+55, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1731.034, 423.1649, -7.883629, 1.543947, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+56, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1936.5, 229.0521, -17.00923, 5.218534, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+57, 14645, 726, 0, 0, 1, 0, 0, 0, 0, 1674.67, -34.0104, 225.9173, 2.251475, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Warsong Gulch Herald (Area: -Unknown- - Difficulty: Normal)
(@CGUID+58, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1777.592, 263.0399, -18.27896, 3.054326, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+59, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1895.812, -5.980329, 41.40893, 1.128013, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal)
(@CGUID+60, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1946.863, 362.0295, -14.45387, 0.418879, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+61, 14645, 726, 0, 0, 1, 0, 0, 0, 0, 1670.92, -34.1024, 224.7903, 0.5585054, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Warsong Gulch Herald (Area: -Unknown- - Difficulty: Normal)
(@CGUID+62, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1976.319, 181.5484, -4.371491, 1.05256, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+63, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1747.47, 241.1632, -13.90293, 3.839724, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+64, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1999.321, 172.1445, 26.67935, 0.02061974, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+65, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1926.684, 219.4648, -17.02013, 2.159363, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+66, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1974.141, 375.4219, -9.842425, 1.884956, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+67, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1787.04, 448.4427, -14.09361, 0.9250245, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+68, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1751.229, 251.5764, -13.90293, 2.530727, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+69, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 2120.229, 363.47, 64.96215, 3.274793, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+70, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1914.78, 359.684, -19.51139, 3.281219, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+71, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1927.949, 230.1389, -17.27105, 4.799771, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+72, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1713.207, 426.4809, -5.051408, 2.617994, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+73, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1945.059, 222, -16.23423, 0.541052, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+74, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1768.049, 411.3681, -13.01563, 3.769911, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+75, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1953.998, 181.7795, -4.440118, 0.03490658, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+76, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1745.186, 418.5451, -7.840154, 5.51524, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal)
(@CGUID+77, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1922.563, 388.8559, -14.39674, 0.2443461, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+78, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1769.95, 226.9879, -13.90292, 5.654867, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+79, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1955.422, 174.1405, -4.412254, 1.682361, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+80, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1765.217, 213.0816, -13.90292, 0.3141593, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+81, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1742.042, 450.5851, -7.749567, 3.368485, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal)
(@CGUID+82, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1956.241, 403.7899, -9.820428, 0.5061455, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+83, 14645, 726, 0, 0, 1, 0, 0, 0, 0, 1675.03, -29.217, 225.8843, 4.24115, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Warsong Gulch Herald (Area: -Unknown- - Difficulty: Normal)
(@CGUID+84, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1767.208, 401.441, -15.22443, 2.181662, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+85, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1972.122, 195.9913, -4.079089, 4.39823, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+86, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1759.196, 431.1667, -8.128761, 3.630285, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+87, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1737.274, 232.3924, -13.90292, 5.969026, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+88, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal)
(@CGUID+89, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1965.139, 184.8164, -4.412254, 5.60417, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+90, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1755.885, 441.5816, -7.899209, 3.892084, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal)
(@CGUID+91, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 2077.476, 287.1659, 98.42361, 2.270585, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+92, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1985.748, 162.4809, 0.5268366, 4.660029, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+93, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 2059.862, 161.0982, 104.5052, 1.625672, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+94, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1985.649, 403.4965, -8.663868, 0.4917996, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+95, 14645, 726, 0, 0, 1, 0, 0, 0, 0, 1676.3, -31.5503, 225.9113, 3.455752, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Warsong Gulch Herald (Area: -Unknown- - Difficulty: Normal)
(@CGUID+96, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1732.887, 200.5521, -9.827867, 5.201081, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+97, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1944.977, 416.2066, -10.00108, 6.056293, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+98, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1739.951, 412.0972, -7.915442, 2.129302, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+99, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1933.481, 202.1754, -16.88152, 4.572762, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+100, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.158301, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal)
(@CGUID+101, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1969.24, 172.7285, -4.371491, 3.139348, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+102, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1931.827, 398.4355, -9.888792, 0.7637421, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+103, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1753.467, 296.0469, -13.16725, 2.460914, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+104, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1738.811, 418.1858, -7.916156, 4.782202, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal)
(@CGUID+105, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1784.918, 457.6371, -12.08598, 1.53589, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+106, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1930.316, 211.658, -16.99481, 1.32645, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: -Unknown- - Difficulty: Normal)
(@CGUID+107, 42615, 726, 0, 0, 1, 0, 0, 0, 0, 1749.061, 419.1945, -8.031966, 4.08407, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Twin Peaks Boar (Area: -Unknown- - Difficulty: Normal)
(@CGUID+108, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1990.781, 395.526, -9.053295, 0.8746758, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+109, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1722.297, 446.8559, -7.749568, 2.408554, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+110, 51126, 726, 0, 0, 1, 0, 0, 0, 0, 1939.223, 238.6864, -14.60192, 1.971911, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon Hatchling (Area: -Unknown- - Difficulty: Normal) (possible waypoints or random movement)
(@CGUID+111, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1747.33, 258.4913, -13.90227, 4.066617, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+112, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1973.71, 168.5017, -4.376678, 4.852015, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+113, 51130, 726, 0, 0, 1, 0, 0, 0, 0, 1726.76, 425.8802, -7.734254, 2.600541, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Dragonmaw Worker (Area: -Unknown- - Difficulty: Normal)
(@CGUID+114, 51128, 726, 0, 0, 1, 0, 0, 0, 0, 1928.988, 411.8125, -12.1624, 0.1047198, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Guard (Area: -Unknown- - Difficulty: Normal)
(@CGUID+117, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+118, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+119, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+120, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+121, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+122, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+123, 14645, 726, 0, 0, 1, 0, 0, 0, 0, 1671.84, -30.8976, 224.8763, 5.480334, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Warsong Gulch Herald (Area: Dragonmaw Flag Room - Difficulty: 0)
(@CGUID+124, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+125, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+126, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+127, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+128, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+129, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+130, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+131, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+132, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+133, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0) (possible waypoints or random movement)
(@CGUID+134, 13117, 726, 0, 0, 1, 0, 0, 0, 0, 1550.53, 211.552, 14.17433, 0.05235988, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Horde Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+135, 13116, 726, 0, 0, 1, 0, 0, 0, 0, 2178.082, 332.7639, 34.06198, 3.263766, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Alliance Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+136, 13117, 726, 0, 0, 1, 0, 0, 0, 0, 1818.306, 153.1302, 1.889904, 1.631285, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Horde Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+137, 13116, 726, 0, 0, 1, 0, 0, 0, 0, 1879.16, 441.9132, -3.916838, 4.18879, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Alliance Spirit Guide (Area: 0 - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+138, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.226, -7.338718, 42.18538, 1.675657, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+139, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+140, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1895.938, 98.54488, 6.829594, 1.411722, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+141, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1927.737, 17.62001, 18.7074, 1.651407, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+142, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+143, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+144, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+145, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+146, 51127, 726, 0, 5775, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Stronghold - Difficulty: 0)
(@CGUID+147, 51127, 726, 0, 5775, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Stronghold - Difficulty: 0)
(@CGUID+148, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+149, 51127, 726, 0, 5775, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Stronghold - Difficulty: 0)
(@CGUID+150, 51127, 726, 0, 5775, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Stronghold - Difficulty: 0)
(@CGUID+151, 51127, 726, 0, 5775, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 10, 0, 0, 0, 1, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Dragonmaw Stronghold - Difficulty: 0) (possible waypoints or random movement)
(@CGUID+152, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+153, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+154, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: Wildhammer Stronghold - Difficulty: 0)
(@CGUID+155, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+156, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+157, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+158, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1890.335, -17.76215, 45.33773, 1.117011, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+159, 51127, 726, 0, 0, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420), -- Wildhammer Gryphon (Area: 0 - Difficulty: 0)
(@CGUID+160, 51127, 726, 0, 5775, 1, 0, 0, 0, 0, 1878.674, -17.76215, 45.33773, 1.204277, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 23420); -- Wildhammer Gryphon (Area: Dragonmaw Stronghold - Difficulty: 0)

UPDATE `creature_template_addon` SET `bytes2`=1 WHERE `entry`=51128; -- 51128 (Wildhammer Guard)

SET @OGUID := 8004000;
DELETE FROM `gameobject` WHERE (`guid` BETWEEN @OGUID+0 AND @OGUID+49) OR `map`=726;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+14, 208207, 726, 0, 0, 1, 0, 0, 1558.088, 372.7654, 1.723727, 6.178466, 0, 0, -0.05233574, 0.9986296, 7200, 255, 1, 23420), -- Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+15, 179871, 726, 0, 0, 1, 0, 0, 2088.865, 258.6302, 43.79683, 1.66559, 0, 0, 0.7398148, 0.6728106, 7200, 255, 1, 23420), -- Speed Buff (Area: -Unknown- - Difficulty: Normal)
(@OGUID+16, 208206, 726, 0, 0, 1, 0, 0, 1574.605, 321.2421, 1.58989, 6.178466, 0, 0, -0.05233574, 0.9986296, 7200, 255, 1, 23420), -- Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+17, 206653, 726, 0, 0, 1, 0, 0, 2135.525, 218.926, 43.60946, 5.750861, 0, 0, -0.263031, 0.9647874, 7200, 255, 1, 23420), -- Wildhammer Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+18, 206655, 726, 0, 0, 1, 0, 0, 2118.088, 154.6754, 43.57089, 2.609261, 0, 0, 0.9647865, 0.2630341, 7200, 255, 1, 23420), -- Wildhammer Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+19, 179905, 726, 0, 0, 1, 0, 0, 1932.832, 226.7917, -17.05979, 2.44346, 0, 0, 0.9396925, 0.3420205, 7200, 255, 1, 23420), -- Berserk Buff (Area: -Unknown- - Difficulty: Normal)
(@OGUID+20, 206654, 726, 0, 0, 1, 0, 0, 2156, 219.2059, 43.6256, 2.609261, 0, 0, 0.9647865, 0.2630341, 7200, 255, 1, 23420), -- Wildhammer Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+21, 179904, 726, 0, 0, 1, 0, 0, 1951.18, 383.795, -10.5257, 4.06662, 0, 0, -0.8949337, 0.4461992, 7200, 255, 1, 23420), -- Food Buff (Area: -Unknown- - Difficulty: Normal)
(@OGUID+22, 179907, 726, 0, 0, 1, 0, 0, 1737.566, 435.8455, -8.086342, 5.515242, 0, 0, -0.3746061, 0.927184, 7200, 255, 1, 23420), -- Berserk Buff (Area: -Unknown- - Difficulty: Normal)
(@OGUID+23, 179906, 726, 0, 0, 1, 0, 0, 1754.163, 242.125, -14.13157, 1.151916, 0, 0, 0.5446386, 0.8386708, 7200, 255, 1, 23420), -- Food Buff (Area: -Unknown- - Difficulty: Normal)
(@OGUID+24, 180322, 726, 0, 0, 1, 0, 0, 1846.399, 513.5538, 30.4798, 0, 0, 0, 0, 1, 7200, 255, 1, 23420), -- Ghost Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+25, 180322, 726, 0, 0, 1, 0, 0, 1820.47, 517.4358, 25.5526, 6.248279, 0, 0, -0.01745224, 0.9998477, 7200, 255, 1, 23420), -- Ghost Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+26, 227741, 726, 0, 0, 1, 0, 0, 2117.637, 191.6823, 44.05199, 6.021387, 0, 0, -0.1305256, 0.9914449, 7200, 255, 1, 23420), -- -Unknown- (Area: -Unknown- - Difficulty: Normal)
(@OGUID+27, 227740, 726, 0, 0, 1, 0, 0, 1578.337, 344.0451, 2.418409, 2.792518, 0, 0, 0.984807, 0.1736523, 7200, 255, 1, 23420), -- -Unknown- (Area: -Unknown- - Difficulty: Normal)
(@OGUID+28, 203710, 726, 0, 0, 1, 0, 0, 1558.623, 379.1595, -6.409669, 4.607672, 0, 0, -0.743144, 0.6691315, 7200, 255, 1, 23420), -- Portcullis (Area: -Unknown- - Difficulty: Normal)
(@OGUID+29, 180322, 726, 0, 0, 1, 0, 0, 1832.01, 516.0191, 26.9934, 6.248279, 0, 0, -0.01745224, 0.9998477, 7200, 255, 1, 23420), -- Ghost Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+30, 179899, 726, 0, 0, 1, 0, 0, 1611.271, 306.9635, 0.2486351, 4.98984, 0, 0, -0.6025343, 0.798093, 7200, 255, 1, 23420), -- Speed Buff (Area: -Unknown- - Difficulty: Normal)
(@OGUID+31, 207075, 726, 0, 0, 1, 0, 0, 1986.051, 420.5457, -20.70723, 5.148723, 0, 0, -0.5372992, 0.8433917, 7200, 255, 1, 23420), -- Cozy Fire (Area: -Unknown- - Difficulty: Normal)
(@OGUID+32, 180322, 726, 0, 0, 1, 0, 0, 1857.8, 512.559, 34.9307, 0.05235888, 0, 0, 0.02617645, 0.9996573, 7200, 255, 1, 23420), -- Ghost Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+33, 207076, 726, 0, 0, 1, 0, 0, 1953.241, 394.6901, -9.542233, 0.5497769, 0, 0, 0.2714396, 0.9624555, 7200, 255, 1, 23420), -- Cookpot (Area: -Unknown- - Difficulty: Normal)
(@OGUID+34, 208205, 726, 0, 0, 1, 0, 0, 1556.656, 314.7127, 1.589001, 6.178466, 0, 0, -0.05233574, 0.9986296, 7200, 255, 1, 23420); -- Gate (Area: -Unknown- - Difficulty: Normal)

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+49;
INSERT INTO `gameobject_addon` (`guid`, `parent_rotation0`, `parent_rotation1`, `parent_rotation2`, `parent_rotation3`) VALUES
(@OGUID+14, 0, 0, -0.05233581, 0.9986295), -- Gate
(@OGUID+16, 0, 0, -0.05233581, 0.9986295), -- Gate
(@OGUID+17, 0, 0, 0.9647873, 0.2630313), -- Wildhammer Gate
(@OGUID+18, 0, 0, 0.9647873, 0.2630313), -- Wildhammer Gate
(@OGUID+20, 0, 0, 0.9647873, 0.2630313), -- Wildhammer Gate
(@OGUID+28, 0, 0, -0.05233581, 0.9986295), -- Portcullis
(@OGUID+31, 0, 0, 0.3907311, 0.9205049), -- Cozy Fire
(@OGUID+33, 0, 0, 0.3907311, 0.9205049), -- Cookpot
(@OGUID+34, 0, 0, -0.05233581, 0.9986295); -- Gate

DELETE FROM `battleground_door` WHERE `BattlegroundEntry`=108;
INSERT INTO `battleground_door` (`BattlegroundEntry`, `GameObjectSpawnid`, `Comment`) VALUES
(108, @OGUID+4014, 'Twin Peaks - Gate'),
(108, @OGUID+4016, 'Twin Peaks - Gate'),
(108, @OGUID+4017, 'Twin Peaks - Wildhammer Gate'),
(108, @OGUID+4018, 'Twin Peaks - Wildhammer Gate'),
(108, @OGUID+4020, 'Twin Peaks - Wildhammer Gate'),
(108, @OGUID+4028, 'Twin Peaks - Portcullis'),
(108, @OGUID+4034, 'Twin Peaks - Gate');

-- Battle for Gilneas
SET @OGUID := 8005000;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+42;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 205036, 761, 0, 0, 1, 0, 0, 927.2473, 1015.525, 19.00166, 2.783795, 0, 0, 0.9840403, 0.177946, 7200, 255, 1, 26365), --  Stove (Area: -Unknown- - Difficulty: Normal)
(@OGUID+1, 205038, 761, 0, 0, 1, 0, 0, 927.7422, 1016.915, 19.86332, 0.2705252, 0, 0, 0.1348505, 0.9908659, 7200, 255, 1, 26365), --  Fire (Area: -Unknown- - Difficulty: Normal)
(@OGUID+2, 267985, 761, 0, 0, 1, 0, 0, 1015.027, 929.0425, 14.26296, 3.263772, 0, 0, -0.9981346, 0.06105176, 7200, 255, 1, 26365), --  Doodad_firewoodpile-indoor-only01 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+3, 207083, 761, 0, 0, 1, 0, 0, 851.2249, 1361.315, 30.00773, 4.136433, 0, 0, -0.8788166, 0.4771597, 7200, 255, 1, 26365), --  Stove (Area: -Unknown- - Difficulty: Normal)
(@OGUID+4, 207082, 761, 0, 0, 1, 0, 0, 885.6921, 1375.176, 29.34805, 1.893679, 0, 0, 0.811573, 0.584251, 7200, 255, 1, 26365), --  Cauldron (Area: -Unknown- - Difficulty: Normal)
(@OGUID+5, 207177, 761, 0, 0, 1, 0, 0, 918.3906, 1336.641, 27.4252, 2.844883, 0, 0, 0.9890156, 0.1478114, 7200, 255, 1, 26365), --  Alliance Door (Area: -Unknown- - Difficulty: Normal)
(@OGUID+6, 267983, 761, 0, 0, 1, 0, 0, 1066.744, 1307.729, 4.978141, 4.476771, 0, 0, -0.7853165, 0.6190946, 7200, 255, 1, 26365), --  Doodad_firewoodpile-indoor-only01 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+7, 207178, 761, 0, 0, 1, 0, 0, 1395.97, 977.0903, 7.63597, 6.274459, 0, 0, -0.00436306, 0.9999905, 7200, 255, 1, 26365), --  Horde Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+11, 205495, 761, 0, 0, 1, 0, 0, 1395.96, 977.257, -13.7897, 6.265733, 0, 0, -0.00872612, 0.9999619, 7200, 255, 1, 26365), --  Horde Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+12, 205496, 761, 0, 0, 1, 0, 0, 918.2986, 1336.49, 20.455, 2.82743, 0, 0, 0.9876881, 0.1564362, 7200, 255, 1, 26365), --  Alliance Gate (Area: -Unknown- - Difficulty: Normal)
(@OGUID+15, 205045, 761, 0, 0, 1, 0, 0, 1196.861, 1022.993, 8.256913, 2.617989, 0, 0, 0.9659252, 0.2588213, 7200, 255, 1, 26365), --  Cauldron (Area: -Unknown- - Difficulty: Normal)
(@OGUID+16, 267985, 761, 0, 0, 1, 0, 0, 933.6484, 1427.446, 28.91886, 4.06662, 0, 0, -0.8949337, 0.4461992, 7200, 255, 1, 26365), --  Doodad_firewoodpile-indoor-only01 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+17, 267984, 761, 0, 0, 1, 0, 0, 1065.859, 1307.032, 4.974267, 5.506516, 0, 0, -0.3786478, 0.9255409, 7200, 255, 1, 26365), --  Doodad_firewoodpile-indoor-only02 (Area: -Unknown- - Difficulty: Normal)
(@OGUID+18, 205037, 761, 0, 0, 1, 0, 0, 926.9202, 1014.368, 19.86332, 0.2268905, 0, 0, 0.1132021, 0.993572, 7200, 255, 1, 26365), --  Fire (Area: -Unknown- - Difficulty: Normal)
(@OGUID+20, 228050, 761, 0, 0, 1, 0, 0, 1057.74, 1278.262, 3.179371, 4.909808, 0, 0, -0.6339798, 0.7733496, 7200, 255, 1, 26365), --  Capture Point (Area: Mines - Difficulty: 0)
(@OGUID+21, 228052, 761, 0, 0, 1, 0, 0, 980.033, 948.7379, 12.73536, 5.877464, 0, 0, -0.2014723, 0.9794942, 7200, 255, 1, 26365), --  Capture Point (Area: Gilnean Coast - Difficulty: 0)
(@OGUID+22, 228053, 761, 0, 0, 1, 0, 0, 1251.009, 958.2691, 5.668475, 5.86425, 0, 0, -0.2079391, 0.9781418, 7200, 255, 1, 26365);

DELETE FROM `gameobject_addon` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+42;
INSERT INTO `gameobject_addon` (`guid`, `parent_rotation0`, `parent_rotation1`, `parent_rotation2`, `parent_rotation3`) VALUES
(@OGUID+0, 0, 0, -0.1736483, 0.9848077), --  Stove
(@OGUID+1, 0, 0, -0.1736483, 0.9848077), --  Fire
(@OGUID+2, 0, 0, 0.9271839, -0.3746066), --  Doodad_firewoodpile-indoor-only01
(@OGUID+3, 0, 0, 0.4656147, 0.8849875), --  Stove
(@OGUID+4, 0, 0, 0.4656147, 0.8849875), --  Cauldron
(@OGUID+6, 0, 0, 0.9117621, -0.4107189), --  Doodad_firewoodpile-indoor-only01
(@OGUID+15, 0, 0, -0.2756374, 0.9612617), --  Cauldron
(@OGUID+16, 0, 0, -0.7071068, 0.7071068), --  Doodad_firewoodpile-indoor-only01
(@OGUID+17, 0, 0, 0.9117621, -0.4107189), --  Doodad_firewoodpile-indoor-only02
(@OGUID+18, 0, 0, -0.1736483, 0.9848077); --  Fire

DELETE FROM `battleground_door` WHERE `BattlegroundEntry`=120;
INSERT INTO `battleground_door` (`BattlegroundEntry`, `GameObjectSpawnid`, `Comment`) VALUES
(120, @OGUID+5005, 'Battle for Gilneas - Alliance Door'),
(120, @OGUID+5007, 'Battle for Gilneas - Horde Gate');

SET @CGUID := 8005000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+260;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+0, 14848, 761, 0, 0, 1, 0, 0, 0, 0, 1073.7, 1166.68, 5.155784, 0.6457718, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Herald (Area: -Unknown- - Difficulty: Normal)
(@CGUID+1, 13117, 761, 0, 0, 1, 0, 0, 0, 0, 1407.43, 977.115, 7.525484, 3.141593, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Waterworks - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+2, 13116, 761, 0, 0, 1, 0, 0, 0, 0, 899.102, 1341.37, 27.65543, 6.038839, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Waterworks - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+6, 13117, 761, 0, 0, 1, 0, 0, 0, 0, 1252.32, 831.594, 27.87283, 1.48353, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Mines - Difficulty: 0) (Auras: 9036 - Ghost)
(@CGUID+7, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1274.545, 884.0521, 14.26013, 5.253441, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+8, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1284.773, 895.0347, 14.6164, 4.08407, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+9, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1229.087, 889.4028, 14.63209, 4.049164, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+10, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1205.057, 1019.384, 7.156617, 6.265732, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+12, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1249.767, 884.8021, 17.73372, 0.7679449, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+13, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1225.569, 950.5208, 6.115173, 4.852015, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+16, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1252.418, 887.0052, 17.46412, 3.926991, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Mines - Difficulty: 0)
(@CGUID+17, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1225.892, 906.0521, 13.06384, 2.600541, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+18, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1240.194, 985.0469, 7.65058, 6.021386, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+20, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1272.495, 866.0989, 13.66302, 6.178465, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+21, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1194.656, 972.7986, 5.722801, 5.654867, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+22, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1207.976, 978.691, 5.801487, 2.478368, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+23, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1242.856, 985.4202, 7.93904, 3.926991, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Mines - Difficulty: 0)
(@CGUID+24, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1195.439, 1004.217, 6.810771, 0.06981317, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+26, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1207.719, 1020.212, 7.489288, 4.171337, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Mines - Difficulty: 0)
(@CGUID+27, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1014.451, 1277.719, 2.993161, 4.415683, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+28, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1082.342, 1301.958, 3.252829, 0.9250245, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+29, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1022.104, 1328.111, 1.78019, 1.48353, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+30, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1064.01, 1299.79, 3.892361, 3.508112, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+31, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1055.625, 1257.108, 5.449193, 3.752458, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+32, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1068.504, 1258.483, 5.109375, 5.846853, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Mines - Difficulty: 0)
(@CGUID+33, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1102.224, 1290.401, 6.491138, 6.195919, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+34, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1070.927, 1258.563, 4.870978, 3.857178, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+35, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1085.677, 1289.865, 3.711826, 1.623156, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+36, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1120.259, 1271.83, 5.32655, 4.729842, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Mines - Difficulty: 0)
(@CGUID+37, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1039.899, 1268.821, 3.62583, 4.363323, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+38, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1065.344, 1309.024, 4.991201, 0.296706, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+39, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1061.729, 1300.609, 3.86882, 5.497787, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Mines - Difficulty: 0)
(@CGUID+40, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1034.814, 1281.826, 2.787798, 4.572762, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+41, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 990.9601, 1330.703, 6.090046, 1.448623, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+42, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1034.639, 1309.823, 2.392376, 2.024582, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+43, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1062.238, 1316.905, 4.994869, 1.448623, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+44, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1121.439, 1269.712, 5.370519, 2.740167, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Mines - Difficulty: 0)
(@CGUID+45, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1032.47, 1257.967, 4.555556, 0.296706, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+46, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1043.358, 1318.629, 3.114259, 2.792527, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+50, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 903.7864, 1008.292, 16.85243, 3.246312, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Mines - Difficulty: 0)
(@CGUID+51, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 956.1094, 921.9531, 13.9484, 4.310963, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+53, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 979.4097, 919.5729, 13.25483, 5.113815, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+54, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 936.5347, 958.5781, 14.31638, 3.473205, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+55, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 949.1979, 925.4792, 15.77679, 5.061455, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+56, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1019.849, 910.0555, 13.26177, 5.864306, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+57, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 981.908, 978.4202, 13.25093, 0.9424778, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+58, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1006.102, 952.8993, 13.24653, 4.991642, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Mines - Difficulty: 0)
(@CGUID+60, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 901.7969, 1009.68, 16.96875, 5.235988, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Mines - Difficulty: 0)
(@CGUID+61, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 997.0295, 933.9896, 13.25483, 6.091199, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+62, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 972.7136, 966.7327, 13.25483, 3.717551, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Mines - Difficulty: 0)
(@CGUID+63, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 947.3663, 971.8611, 13.72722, 1.570796, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Mines - Difficulty: 0)
(@CGUID+64, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1007.686, 951.0729, 13.25347, 3.001966, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Mines - Difficulty: 0)
(@CGUID+66, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 970.3281, 967.1684, 13.25483, 5.707227, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Mines - Difficulty: 0)
(@CGUID+67, 13116, 761, 0, 0, 1, 0, 0, 0, 0, 886.377, 937.208, 24.37353, 0.3316126, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Lighthouse - Difficulty: 0) (Auras: 9036 - Ghost, 9617 - Ghost Visual, 22011 - Spirit Heal Channel)
(@CGUID+68, 13116, 761, 0, 0, 1, 0, 0, 0, 0, 1035.88, 1341.28, 11.62283, 4.415683, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Alliance Spirit Guide (Area: Horde Landing - Difficulty: 0) (Auras: 9036 - Ghost)
(@CGUID+69, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1070.927, 1258.563, 4.870978, 3.857178, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+70, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1082.342, 1301.958, 3.252829, 0.9250245, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+71, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1068.504, 1258.483, 5.109375, 5.846853, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+72, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1055.625, 1257.108, 5.449193, 3.752458, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+73, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1121.439, 1269.712, 5.370519, 2.740167, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+74, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1102.224, 1290.401, 6.491138, 6.195919, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+75, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1120.259, 1271.83, 5.32655, 4.729842, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+76, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1085.677, 1289.865, 3.711826, 1.623156, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+77, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 990.9601, 1330.703, 6.090046, 1.448623, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+78, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1064.01, 1299.79, 3.892361, 3.508112, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+79, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1065.344, 1309.024, 4.991201, 0.296706, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+80, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1014.451, 1277.719, 2.993161, 4.415683, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+81, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1043.358, 1318.629, 3.114259, 2.792527, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+82, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1034.639, 1309.823, 2.392376, 2.024582, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+83, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1022.104, 1328.111, 1.78019, 1.48353, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+84, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1061.729, 1300.609, 3.86882, 5.497787, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+85, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1062.238, 1316.905, 4.994869, 1.448623, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+86, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1034.814, 1281.826, 2.787798, 4.572762, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+87, 41913, 761, 0, 0, 1, 0, 0, 0, 0, 1032.47, 1257.967, 4.555556, 0.296706, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Gilneas Citizen (Area: Horde Landing - Difficulty: 0)
(@CGUID+88, 41915, 761, 0, 0, 1, 0, 0, 0, 0, 1039.899, 1268.821, 3.62583, 4.363323, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Worgen Warrior (Area: Horde Landing - Difficulty: 0)
(@CGUID+89, 13117, 761, 0, 0, 1, 0, 0, 0, 0, 1035.729, 1341.311, 11.62813, 4.45059, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Lighthouse - Difficulty: 0) (Auras: 9036 - Ghost)
(@CGUID+125, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1238.854, 941.5695, 6.62732, 3.333579, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Mines - Difficulty: 0)
(@CGUID+195, 13117, 761, 0, 0, 1, 0, 0, 0, 0, 886.45, 936.668, 24.44873, 0.4537856, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Horde Spirit Guide (Area: Waterworks - Difficulty: 0) (Auras: 9036 - Ghost)
(@CGUID+196, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1019.849, 910.0555, 13.26177, 5.864306, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Waterworks - Difficulty: 0)
(@CGUID+198, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 947.3663, 971.8611, 13.72722, 1.570796, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+199, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 860.2014, 979.6215, 19.22939, 4.520403, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+200, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 981.908, 978.4202, 13.25093, 0.9424778, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Waterworks - Difficulty: 0)
(@CGUID+201, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 956.1094, 921.9531, 13.9484, 4.310963, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+203, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 1007.686, 951.0729, 13.25347, 3.001966, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Waterworks - Difficulty: 0)
(@CGUID+204, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 979.4097, 919.5729, 13.25483, 5.113815, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+206, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 997.0295, 933.9896, 13.25483, 6.091199, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+207, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 901.7969, 1009.68, 16.96875, 5.235988, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Waterworks - Difficulty: 0)
(@CGUID+208, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 972.7136, 966.7327, 13.25483, 3.717551, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Waterworks - Difficulty: 0)
(@CGUID+209, 42053, 761, 0, 0, 1, 0, 0, 0, 0, 903.7864, 1008.292, 16.85243, 3.246312, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Plaguebearer (Area: Waterworks - Difficulty: 0)
(@CGUID+212, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 970.3281, 967.1684, 13.25483, 5.707227, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Waterworks - Difficulty: 0)
(@CGUID+213, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 949.1979, 925.4792, 15.77679, 5.061455, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+214, 41914, 761, 0, 0, 1, 0, 0, 0, 0, 1006.102, 952.8993, 13.24653, 4.991642, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Reaver (Area: Waterworks - Difficulty: 0)
(@CGUID+215, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 936.5347, 958.5781, 14.31638, 3.473205, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Waterworks - Difficulty: 0)
(@CGUID+243, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 919.7222, 1011.401, 19.43074, 1.23839, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Gilnean Coast - Difficulty: 0)
(@CGUID+244, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 961.7934, 977.441, 14.23678, 4.520403, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Gilnean Coast - Difficulty: 0)
(@CGUID+246, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 917.9479, 1012.205, 19.43074, 2.952153, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Gilnean Coast - Difficulty: 0)
(@CGUID+249, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1022.205, 918.5087, 13.25483, 1.361989, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365), -- Forsaken Invader (Area: Gilnean Coast - Difficulty: 0)
(@CGUID+250, 41911, 761, 0, 0, 1, 0, 0, 0, 0, 1012.12, 933.3403, 14.27748, 5.283335, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 26365); -- Forsaken Invader (Area: Gilnean Coast - Difficulty: 0)

DELETE FROM `battleground_capture_point_creature` WHERE `BattlegroundEntry`=120;
INSERT INTO `battleground_capture_point_creature` (`BattlegroundEntry`, `CapturePointKey`, `SpawnId`, `TeamId`, `Comment`) VALUES
(120, 228050, @CGUID+027, 1, 'Battle for Gilneas - Lighthouse - Forsaken Plaguebearer'),
(120, 228050, @CGUID+028, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+029, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+030, 1, 'Battle for Gilneas - Lighthouse - Forsaken Plaguebearer'),
(120, 228050, @CGUID+031, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+032, 1, 'Battle for Gilneas - Lighthouse - Forsaken Reaver'),
(120, 228050, @CGUID+033, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+034, 1, 'Battle for Gilneas - Lighthouse - Forsaken Plaguebearer'),
(120, 228050, @CGUID+035, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+036, 1, 'Battle for Gilneas - Lighthouse - Forsaken Reaver'),
(120, 228050, @CGUID+037, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+038, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+039, 1, 'Battle for Gilneas - Lighthouse - Forsaken Reaver'),
(120, 228050, @CGUID+040, 1, 'Battle for Gilneas - Lighthouse - Forsaken Plaguebearer'),
(120, 228050, @CGUID+041, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+042, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+043, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+044, 1, 'Battle for Gilneas - Lighthouse - Forsaken Plaguebearer'),
(120, 228050, @CGUID+045, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+046, 1, 'Battle for Gilneas - Lighthouse - Forsaken Invader'),
(120, 228050, @CGUID+068, 0, 'Battle for Gilneas - Lighthouse - Alliance Spirit Guide'),
(120, 228050, @CGUID+069, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+070, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+071, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+072, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+073, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+074, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+075, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+076, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+077, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+078, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+079, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+080, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+081, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+082, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+083, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+084, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+085, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+086, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+087, 0, 'Battle for Gilneas - Lighthouse - Gilneas Citizen'),
(120, 228050, @CGUID+088, 0, 'Battle for Gilneas - Lighthouse - Worgen Warrior'),
(120, 228050, @CGUID+089, 1, 'Battle for Gilneas - Lighthouse - Horde Spirit Guide'),
(120, 228052, @CGUID+050, 0, 'Battle for Gilneas - Waterworks - Worgen Warrior'),
(120, 228052, @CGUID+051, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+053, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+054, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+055, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+056, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+057, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+058, 0, 'Battle for Gilneas - Waterworks - Worgen Warrior'),
(120, 228052, @CGUID+060, 0, 'Battle for Gilneas - Waterworks - Worgen Warrior'),
(120, 228052, @CGUID+061, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+062, 0, 'Battle for Gilneas - Waterworks - Worgen Warrior'),
(120, 228052, @CGUID+063, 0, 'Battle for Gilneas - Waterworks - Gilneas Citizen'),
(120, 228052, @CGUID+064, 0, 'Battle for Gilneas - Waterworks - Worgen Warrior'),
(120, 228052, @CGUID+066, 0, 'Battle for Gilneas - Waterworks - Worgen Warrior'),
(120, 228052, @CGUID+067, 0, 'Battle for Gilneas - Waterworks - Alliance Spirit Guide'),
(120, 228052, @CGUID+195, 1, 'Battle for Gilneas - Waterworks - Horde Spirit Guide'),
(120, 228052, @CGUID+196, 1, 'Battle for Gilneas - Waterworks - Forsaken Plaguebearer'),
(120, 228052, @CGUID+198, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+199, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+200, 1, 'Battle for Gilneas - Waterworks - Forsaken Reaver'),
(120, 228052, @CGUID+201, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+203, 1, 'Battle for Gilneas - Waterworks - Forsaken Plaguebearer'),
(120, 228052, @CGUID+204, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+206, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+207, 1, 'Battle for Gilneas - Waterworks - Forsaken Reaver'),
(120, 228052, @CGUID+208, 1, 'Battle for Gilneas - Waterworks - Forsaken Plaguebearer'),
(120, 228052, @CGUID+209, 1, 'Battle for Gilneas - Waterworks - Forsaken Plaguebearer'),
(120, 228052, @CGUID+212, 1, 'Battle for Gilneas - Waterworks - Forsaken Reaver'),
(120, 228052, @CGUID+213, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+214, 1, 'Battle for Gilneas - Waterworks - Forsaken Reaver'),
(120, 228052, @CGUID+215, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+243, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+244, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+246, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+249, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228052, @CGUID+250, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader'),
(120, 228053, @CGUID+006, 1, 'Battle for Gilneas - Mines - Horde Spirit Guide'),
(120, 228053, @CGUID+007, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+008, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+009, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+010, 1, 'Battle for Gilneas - Mines - Forsaken Plaguebearer'),
(120, 228053, @CGUID+012, 1, 'Battle for Gilneas - Mines - Forsaken Plaguebearer'),
(120, 228053, @CGUID+013, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+016, 1, 'Battle for Gilneas - Mines - Forsaken Reaver'),
(120, 228053, @CGUID+017, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+018, 1, 'Battle for Gilneas - Mines - Forsaken Plaguebearer'),
(120, 228053, @CGUID+020, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+021, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+022, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+023, 1, 'Battle for Gilneas - Mines - Forsaken Reaver'),
(120, 228053, @CGUID+024, 1, 'Battle for Gilneas - Mines - Forsaken Invader'),
(120, 228053, @CGUID+026, 1, 'Battle for Gilneas - Mines - Forsaken Reaver'),
(120, 228053, @CGUID+125, 1, 'Battle for Gilneas - Waterworks - Forsaken Invader');

-- Seething Shore
DELETE FROM `gameobject_template` WHERE `entry` IN (281307, 272471, 278407, 279254);
INSERT INTO `gameobject_template` VALUES
(281307, 42, 47685, 'Azerite', 'mine', '', '', 1.1, 60000, 1, 1, 2825, 134113, 0, 0, 134114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68590, 0, 0, 0, 0, 2405, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 26124),
(272471, 42, 47686, 'Azerite', 'mine', '', '', 1.1, 60000, 1, 1, 2825, 134113, 0, 0, 134114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68590, 0, 0, 0, 0, 2405, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 26124),
(278407, 15, 9002, 'Sword of Dawn', '', '', '', 1, 6519, 4, 1, 0, 0, 0, 1856, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 26124),
(279254, 15, 9001, 'The Warbringer', '', '', '', 1, 6520, 4, 1, 0, 0, 0, 1857, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 26124);
DELETE FROM `gameobject_template` WHERE `entry` IN (281224 /*Alliance Airship Prep Collision*/, 281226 /*Horde Airship Prep Collision*/, 281026 /*War Drums*/);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `RequiredLevel`, `VerifiedBuild`) VALUES
(281224, 0, 47611, 'Alliance Airship Prep Collision', '', '', '', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Alliance Airship Prep Collision
(281226, 0, 47612, 'Horde Airship Prep Collision', '', '', '', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124), -- Horde Airship Prep Collision
(281026, 5, 20408, 'War Drums', 'questinteract', '', '', 0.75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26124); -- War Drums

DELETE FROM `gameobject_template_addon` WHERE `entry` IN (279254, 278407, 281307 /*Azerite*/, 272471 /*Azerite*/, 281224 /*Alliance Airship Prep Collision*/, 281226 /*Horde Airship Prep Collision*/);
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `WorldEffectID`) VALUES
(278407, 0, 1048616, 0, 0, 0),
(279254, 0, 1048616, 0, 0, 0),
(281307, 0, 32, 0, 0, 0), -- Azerite
(272471, 0, 32, 0, 0, 0), -- Azerite
(281224, 1375, 32, 0, 0, 0), -- Alliance Airship Prep Collision
(281226, 1375, 0, 0, 0, 0); -- Horde Airship Prep Collision

DELETE FROM `creature_equip_template` WHERE (`CreatureID`=88239 AND `ID`=1) OR (`CreatureID`=131776 AND `ID`=1) OR (`CreatureID`=133048 AND `ID`=1) OR (`CreatureID`=133049 AND `ID`=1) OR (`CreatureID`=130677 AND `ID`=1) OR (`CreatureID`=133045 AND `ID`=1) OR (`CreatureID`=133043 AND `ID`=1) OR (`CreatureID`=131773 AND `ID`=1) OR (`CreatureID`=133059 AND `ID`=1) OR (`CreatureID`=93493 AND `ID`=1);
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`) VALUES
(88239, 1, 13337, 0, 0, 0, 0, 0, 0, 0, 0), -- Horde Spirit Guide
(131776, 1, 76364, 0, 0, 0, 0, 0, 0, 0, 0), -- Jeron Emberfall
(133048, 1, 1911, 0, 0, 0, 0, 0, 0, 0, 0), -- Tevs Rusthammer
(133049, 1, 107367, 0, 0, 0, 0, 0, 2550, 0, 0), -- Gunship Warguard
(130677, 1, 56912, 0, 0, 56912, 0, 0, 106217, 0, 0), -- Gunship Grunt
(133045, 1, 3350, 0, 0, 3350, 0, 0, 0, 0, 0), -- War Drummer
(133043, 1, 0, 0, 0, 0, 0, 0, 2550, 0, 0), -- High Warlord Volrath
(131773, 1, 65795, 0, 0, 0, 0, 0, 5258, 0, 0), -- Nathanos Blightcaller
(133059, 1, 108925, 0, 0, 0, 0, 0, 107953, 0, 0), -- General Harris
(93493, 1, 0, 0, 0, 0, 0, 0, 94714, 0, 0); -- Goblin Sky Bomber

DELETE FROM `gossip_menu` WHERE (`MenuId`=22094 AND `TextId`=33831) OR (`MenuId`=22242 AND `TextId`=34080);
INSERT INTO `gossip_menu` (`MenuId`, `TextId`) VALUES
(22094, 33831), -- 131776 (Jeron Emberfall)
(22242, 34080); -- 133048 (Tevs Rusthammer)

DELETE FROM `gossip_menu_option` WHERE (`MenuId`=22094 AND `OptionIndex`=0);
INSERT INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`) VALUES
(22094, 0, 0, 'Could you conjure me some food?', 0);

DELETE FROM `npc_text` WHERE `ID` IN (34080 /*34080*/, 33831 /*33831*/);
INSERT INTO `npc_text` (`ID`, `Probability0`, `Probability1`, `Probability2`, `Probability3`, `Probability4`, `Probability5`, `Probability6`, `Probability7`, `BroadcastTextId0`, `BroadcastTextId1`, `BroadcastTextId2`, `BroadcastTextId3`, `BroadcastTextId4`, `BroadcastTextId5`, `BroadcastTextId6`, `BroadcastTextId7`, `VerifiedBuild`) VALUES
(34080, 1, 0, 0, 0, 0, 0, 0, 0, 147680, 0, 0, 0, 0, 0, 0, 0, 26124), -- 34080
(33831, 1, 0, 0, 0, 0, 0, 0, 0, 146214, 0, 0, 0, 0, 0, 0, 0, 26124); -- 33831


SET @CGUID := 8009000;
SET @OGUID := 8009000;

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+2;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+0, 281026, 1857, 0, 0, 3, 0, 0, 9.989483, -18.6586, 34.79845, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- War Drums (Area: -Unknown- - Difficulty: 0)
(@OGUID+1, 281226, 1857, 0, 0, 3, 0, 0, 1, -1, 1, 0, 0, 0, 0, 1, 7200, 255, 1, 26124), -- Horde Airship Prep Collision (Area: -Unknown- - Difficulty: 0)
(@OGUID+2, 281224, 1856, 0, 0, 3, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 7200, 255, 1, 26124); -- Alliance Airship Prep Collision (Area: -Unknown- - Difficulty: 0)

DELETE FROM `battleground_door` WHERE `BattlegroundEntry`=894;
INSERT INTO `battleground_door` (`BattlegroundEntry`, `GameObjectSpawnId`, `Comment`) VALUES
(894, 8009001, 'Seething Shore - Horde Airship Prep Collision'),
(894, 8009002, 'Seething Shore - Alliance Airship Prep Collision');

DELETE FROM `creature_model_info` WHERE `DisplayID`=82685;
INSERT INTO `creature_model_info` VALUES
(82685, 1, 1, 0, 0);

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_bg_seething_shore_activate_azerite', 'aura_bg_seething_shore_azerite_geyser');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(248688, 'spell_bg_seething_shore_activate_azerite'),
(248668, 'aura_bg_seething_shore_azerite_geyser');

UPDATE `creature_template` SET `ScriptName`='npc_bg_seething_shore_azerite_fissure' WHERE `entry`=125253;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=248688 AND `ConditionTypeOrReference`=31;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 248688, 31, 3, 125253, 'Activate Azerite - Only Target Azerute Fissure');
