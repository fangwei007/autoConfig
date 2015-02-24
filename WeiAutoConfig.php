<?php

class WeiAutoConfig {

    public $hostname;
    public $dbAdapter;
    public $dbUser;
    public $dbPwd;
    public $dbName;
    public $location;
    public $serverName = array();
    public $serverString;

    public function __construct() {
        ;
    }

    public function _init() {
        fwrite(STDOUT, "添加一个新配置？(Y/n)");
        $arg = trim(fgets(STDIN));
        $answer = array("Y", 'y', 'yes', 'Yes', '');
        if (in_array($arg, $answer)) {
            $this->addLocation();
            $this->addHeader();
            $this->multiCtl();
            $this->addFooter();
            echo "配置完成，谢谢使用！\n";
        }
    }

    public function multiCtl() {
        do {
            $this->addHost();
            $this->addDbUser();
            $this->addDbPwd();
            $this->addDbName();
            $this->addServerName();
            $this->writeConfig();
            fwrite(STDOUT, "添加另一个新配置？(Y/n)");
            $arg = trim(fgets(STDIN));
            $answer = array("Y", 'y', 'yes', 'Yes', '');
        } while (in_array($arg, $answer));
    }

    public function addHost() {
        fwrite(STDOUT, "请输入HOST：");
        $this->hostname = trim(fgets(STDIN));
    }

    public function addDbUser() {
        fwrite(STDOUT, "请输入DB User：");
        $this->dbUser = trim(fgets(STDIN));
    }

    public function addDbPwd() {
        fwrite(STDOUT, "请输入DB Password：");
        $this->dbPwd = trim(fgets(STDIN));
    }

    public function addDbName() {
        fwrite(STDOUT, "请输入DB Name：");
        $this->dbName = trim(fgets(STDIN));
    }

    public function addServerName() {
        $i = 1;
        do {
            fwrite(STDOUT, "请输入Server Name $i (Press 'enter' quit)：");
            $server = trim(fgets(STDIN));
            $this->serverName[$i - 1] = $server;
            $i++;
        } while ($server !== '');
        $this->parseCondition();
//        var_dump($this->serverName);
    }

    private function parseCondition() {
        $condition = 'if(';
        if (count($this->serverName) > 1) {
            $condition .= "\$_SERVER['SERVER_NAME'] === '{$this->serverName[0]}'";
            for ($i = 1; $i < count($this->serverName) - 1; $i++) {
                $condition .= " || \$_SERVER['SERVER_NAME'] ===  '{$this->serverName[$i]}'";
            }
            $condition .= ' )';
            $this->serverString = $condition;
        } else {
            $this->serverString = empty($this->serverName) ? "if( \$_SERVER['SERVER_NAME'] === " : "if( \$_SERVER['SERVER_NAME'] === '{$this->serverName[0]}' )";
        }
//        var_dump($this->serverString);
    }

    public function addLocation() {
        fwrite(STDOUT, "请输入配置文件目录地址：");
        $this->location = trim(fgets(STDIN)) . '/config.php';
    }

    public function addHeader() {
        if (file_exists($this->location)) {
            echo "文件已经存在！\n";
            exit();
        } else {
            $handle = fopen($this->location, 'a+');
            $header = '<?php';
            if (is_writable($this->location)) {
                fwrite($handle, $header);
            }
            fclose($handle);
        }
    }

    public function addFooter() {
        $handle = fopen($this->location, 'a+');
        $footer = <<<EOT
                
	\$mysqli = new mysqli(DB_SERVER,DB_USER,DB_PASS,DB_NAME);
	if (\$mysqli->connect_errno) {
	    die("Failed to connect to MySQL: (" . \$mysqli->connect_errno . ") " . \$mysqli->connect_error);
	}
EOT;
        if (is_writable($this->location)) {
            fwrite($handle, $footer);
        }
        fclose($handle);
    }

    public function writeConfig() {
        $handle = fopen($this->location, 'a+');
        $configString = <<<EOT

        $this->serverString
        {
            define("ENV", "local");
            define("BASE_URL", "http://local.endgameiscoming.dev");
            define("RETURN_URL", "http://local.endgameiscoming.dev");
            define("FB_APP", "758786784162393");
            define("CDN_URL","video/");
            define("DB_SERVER", '$this->hostname');
            define("DB_USER", '$this->dbUser');
            define("DB_PASS", '$this->dbPwd');
            define("DB_NAME", '$this->dbName');
	}
EOT;
        if (is_writable($this->location)) {
            fwrite($handle, $configString);
        }
        fclose($handle);
    }

}

$autoconfig = new WeiAutoConfig();
$autoconfig->_init();
