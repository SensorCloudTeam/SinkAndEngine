-- phpMyAdmin SQL Dump
-- version 3.3.7
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2015 �?03 �?30 �?16:18
-- 服务器版本: 5.6.21
-- PHP 版本: 5.5.18

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `wsn`
--

-- --------------------------------------------------------

--
-- 表的结构 `sensor`
--

CREATE TABLE IF NOT EXISTS `sensor` (
  `id` varchar(26) NOT NULL COMMENT '传感器唯一标识，结构为 sink_id+''_''+sensor_id',
  `sensor_id` int(5) NOT NULL COMMENT '一个发布节点分配给传感器的内部id',
  `sink_id` varchar(20) NOT NULL COMMENT '外键，指向sink表中的id',
  `name` varchar(100) NOT NULL COMMENT '传感器名字',
  `data_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '接受数据的时间',
  `sample_rate` int(11) DEFAULT NULL COMMENT '接受数据的频率(s/次)',
  `type_id` int(11) DEFAULT NULL COMMENT '外键，指向type表中的id',
  `value` varchar(10000) DEFAULT NULL COMMENT '传感器的数据',
  `post` tinyint(1) DEFAULT NULL COMMENT '是否对外发布',
  PRIMARY KEY (`id`),
  KEY `sink_id` (`sink_id`),
  KEY `type_id` (`type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `sensor`
--


-- --------------------------------------------------------

--
-- 表的结构 `sink`
--

CREATE TABLE IF NOT EXISTS `sink` (
  `id` varchar(20) NOT NULL COMMENT '发布节点的唯一标识，由服务器随机分配，等同于序列号',
  `user_id` varchar(14) DEFAULT NULL COMMENT '外键，指向user表中的id',
  `name` varchar(100) NOT NULL COMMENT '节点名字',
  `longitude` decimal(12,9) DEFAULT NULL COMMENT '经度，小数点后保留9位，范围【-180~180】',
  `latitude` decimal(11,9) DEFAULT NULL COMMENT '纬度，小数点后保留9位，范围【-90~90】',
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `sink`
--


-- --------------------------------------------------------

--
-- 表的结构 `subscription`
--

CREATE TABLE IF NOT EXISTS `subscription` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '订阅编号',
  `sensor_id` varchar(26) NOT NULL COMMENT '要订阅的传感器id',
  `user_id` varchar(14) NOT NULL COMMENT '订阅人的id',
  `sub_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '订阅发起的时间',
  `send_frequency` int(11) DEFAULT NULL COMMENT '推送频率',
  `address` varchar(255) NOT NULL COMMENT '订阅信息的地址,地址可以是邮箱，手机号',
  `filter` int(11) NOT NULL COMMENT '过滤条件',
  `threshold_value` float DEFAULT NULL COMMENT '过滤阈值',
  PRIMARY KEY (`id`),
  KEY `sensor_id` (`sensor_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

--
-- 转存表中的数据 `subscription`
--


-- --------------------------------------------------------

--
-- 表的结构 `type`
--

CREATE TABLE IF NOT EXISTS `type` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '传感器类型的标识',
  `name` varchar(20) NOT NULL COMMENT '传感器类型',
  `unit_symbol` varchar(50) NOT NULL COMMENT '数据的单位符号，比如温度传感器单位为‘℃’',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=9 ;

--
-- 转存表中的数据 `type`
--

INSERT INTO `type` (`id`, `name`, `unit_symbol`) VALUES
(1, '温度', '℃'),
(2, '湿度', 'g/m3'),
(3, 'PH值', ''),
(4, '光照强度', 'Lux'),
(5, '车流量', 'm/s'),
(6, '图片', ''),
(7, '视频', ''),
(8, '自定义数据类型', '');

-- --------------------------------------------------------

--
-- 表的结构 `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `id` varchar(14) NOT NULL COMMENT '用户的唯一标识，有用户注册时提供',
  `password` varchar(100) NOT NULL COMMENT '密码（6-15位）',
  `email` varchar(255) DEFAULT NULL COMMENT '用户关联邮箱',
  `reg_time` varchar(20) NOT NULL COMMENT '注册的时间',
  `poster` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否为发布方，false表示不是',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `user`
--

INSERT INTO `user` (`id`, `password`, `email`, `reg_time`, `poster`) VALUES
('admin', 'admin', NULL, '2014-10-28 10:00:00', 0),
('lijinyang', 'd430bdab3df8b62cf1b5a620932d77ee604cb068', '544961154@qq.com', '1419236389', 1),
('test_user', '7c4a8d09ca3762af61e59520943dc26494f8941b', '5449611544@qq.com', '1420806238', 0);

--
-- 限制导出的表
--

--
-- 限制表 `sensor`
--
ALTER TABLE `sensor`
  ADD CONSTRAINT `sensor_ibfk_3` FOREIGN KEY (`sink_id`) REFERENCES `sink` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `sensor_ibfk_4` FOREIGN KEY (`type_id`) REFERENCES `type` (`id`) ON DELETE CASCADE;

--
-- 限制表 `sink`
--
ALTER TABLE `sink`
  ADD CONSTRAINT `sink_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE CASCADE;

--
-- 限制表 `subscription`
--
ALTER TABLE `subscription`
  ADD CONSTRAINT `subscription_ibfk_3` FOREIGN KEY (`sensor_id`) REFERENCES `sensor` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `subscription_ibfk_4` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE CASCADE;
