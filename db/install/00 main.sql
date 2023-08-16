SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";

--
--
--


CREATE DATABASE IF NOT EXISTS `tegia_core` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
USE `tegia_core`;


-- ------------------------------------------------------------------------------------------
--
-- Table structure for table `_sessions`
--
-- ------------------------------------------------------------------------------------------

DROP TABLE IF EXISTS `_sessions`;
CREATE TABLE IF NOT EXISTS `_sessions` (
	`_id` bigint NOT NULL AUTO_INCREMENT,
	`sid` varchar(250) NOT NULL,
	`t_create` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`t_update` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
	`id_user` int NOT NULL,
	`user_agent` varchar(250) NOT NULL,
	`user_ip` varchar(250) NOT NULL,
	`data` json DEFAULT NULL,
	PRIMARY KEY (`_id`),
	KEY `sid` (`sid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


--
--
--

COMMIT;