ALTER TABLE realmd_db_version CHANGE COLUMN required_14027_01_realmd_account_logons required_14028_01_realmd_account_locale_agnostic bit;

ALTER TABLE account DROP `locale`;
ALTER TABLE account ADD `locale` VARCHAR(4) NOT NULL DEFAULT '' AFTER `mutetime`;
