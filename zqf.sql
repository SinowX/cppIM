DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`(
    `index` int not null AUTO_INCREMENT,
    `id` varchar(64) not null,
    `username` varchar(64) not null,
    `password` varchar(64) not null,
    `createtime` timestamp DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`index`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DROP TABLE IF EXISTS `message_log`;
CREATE TABLE `message_log`(
    `index` int not null AUTO_INCREMENT,
    `sender_id` varchar(64) not null,
    `receiver_id` varchar(64) not null,
    `content` varchar(255) not null,
    `timestamp` timestamp DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`index`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;