<?php
/**
 * Created by PhpStorm.
 * User: Marty
 * Date: 12-Mar-17
 * Time: 19:57
 */

require "parser.php";
require_once "Scanner.php";

$options = "n";
$longopts = array(
    "help",
    "input:",
    "output:",
    "query:",
    "qf:",
    "root:",
);

$argArray = getopt( $options, $longopts );

if ( isset( $argArray["help"] ) ) {
    if ( $argc > 2 ) {
        fwrite( STDERR, "Invalid script parameters!\nHelp cannot be combined with other parameters.\n" );
        exit ( 1 );
    } else {
        // TODO write actual help dialog
        echo "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n";
        exit( 0 );
    }
}

if ( !(isset( $argArray["query"] ) xor isset( $argArray["qf"] )) ) {
    fwrite( STDERR, "Exactly one query source must be specified.\n" );
    exit( 1 );
}

if ( isset( $argArray["input"] ) ) {
    @$xmlstructure = simplexml_load_file($argArray["input"]);
    if ( !$xmlstructure ) {
        fwrite( STDERR, "Cannot open the input file.\n" );
        exit ( 2 );
    }
} else {
    $xmlstructure = new SimpleXMLElement(STDIN);
}

if ( isset( $argArray["output"] ) ) {
    @$outputFile = fopen( $argArray["output"], "w" );
    if ( !$outputFile ) {
        fwrite( STDERR, "Cannot open the output file.\n" );
        exit ( 3 );
    }
} else {
    $outputFile = STDOUT;
}

$generateHeader = 1;
$query = "";
$root = "";

foreach ( $argArray as $key => $value ) {
    switch ( $key ) {
        case "n":
            $generateHeader = 0;
            break;
        case "query":
            $query = $argArray["query"];
            break;
        case "qf":
            @$queryFile = fopen( $argArray["qf"], "r" );
            if ( !$queryFile ) {
                fwrite( STDERR, "Cannot open the query file .\n" );
                exit ( 2 );
            }
            $query = file_get_contents( $argArray["qf"] );
            fclose( $argArray["qf"] );
            break;
        case "root":
            $root = $argArray["root"];
            break;
        case "input": // kept here just to check validity of parameters, the input is resolved above the foreach statement
        case "output":
            break;
        default:
            fwrite( STDERR, "Invalid script parameters!\nUse --help for usage details.\n" );
            exit ( 1 );
    }
}
echo $query . "\n";

$test = parseQuery($query);
var_dump($test);

var_dump($xmlstructure);

fclose( $outputFile );

exit( 0 );