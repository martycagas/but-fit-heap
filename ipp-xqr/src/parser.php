<?php
/**
 * Created by PhpStorm.
 * User: Marty
 * Date: 14-Mar-17
 * Time: 11:03
 */

require "Scanner.php";

$queryParameters = array(
    "SELECT_ELEM" => "",
    "FROM_ELEM" => "",
    "FROM_ATTR" => "",
    "WHERE_ELEM" => "",
    "WHERE_ATTR" => "",
    "WHERE_RELATION" => "",
    "WHERE_LITERAL" => "",
    "LIMIT" => "",
    "CONDITION" => TRUE,
);

function parseQuery( $query )
{
    global $queryParameters;

    $lex = new Scanner( $query );

    QUERY( $lex );

    $lex->invalidateToken();
    if ( $lex->getToken()["type"] === Scanner::EOF ) {
        return $queryParameters;
    } else {
        fwrite( STDERR, "Query error!\nDetails: unknown tokens present in the query\n" );
        exit( 80 );
    }
}

function QUERY( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::SELECT ) {
        $lex->invalidateToken();
    } else {
        fwrite( STDERR, "Query error!\nDetails: missing SELECT\n" );
        exit( 80 );
    }

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::IDENTIFIER ) {
        $queryParameters["SELECT_ELEM"] = $tok["value"];
        $lex->invalidateToken();
    } else {
        fwrite( STDERR, "Query error!\nDetails: missing SELECT_ELEMENT\n" );
        exit( 80 );
    }

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::FROM ) {
        $lex->invalidateToken();
    } else {
        fwrite( STDERR, "Query error!\nDetails: missing FROM\n" );
        exit( 80 );
    }

    FROM_ELM( $lex );
    WHERE_CLAUSE( $lex );
    LIMIT_n( $lex );
}

function LIMIT_n( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::LIMIT ) {
        $lex->invalidateToken();

        $tok = $lex->getToken();
        if ( $tok["type"] === Scanner::NUMBER ) {
            $queryParameters["LIMIT"] = $tok["value"];
            $lex->invalidateToken();
        } else {
            fwrite( STDERR, "Query error!\nDetails: missing integer after LIMIT\n" );
            exit( 80 );
        }
    }
}

function FROM_ELM( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::ROOT ) {
        $queryParameters["FROM_ELEM"] = "ROOT";
        $lex->invalidateToken();
    } else {
        FROM_ELEMENT_OR_ATTRIBUTE( $lex );
    }
}

function FROM_ELEMENT_OR_ATTRIBUTE( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::IDENTIFIER ) {
        $queryParameters["FROM_ELEM"] = $tok["value"];
        $lex->invalidateToken();

        if ( $lex->getToken()["type"] === Scanner::DOT ) {
            $lex->invalidateToken();

            $tok = $lex->getToken();
            if ( $tok["type"] === Scanner::IDENTIFIER ) {
                $queryParameters["FROM_ATTR"] = $tok["value"];
                $lex->invalidateToken();
            } else {
                fwrite( STDERR, "Query error!\nDetails: element identifier containing DOT character" );
                exit( 80 );
            }
        }
    } elseif ( $tok["type"] === Scanner::DOT ) {
        $lex->invalidateToken();

        $tok = $lex->getToken();
        if ( $tok["type"] === Scanner::IDENTIFIER ) {
            $queryParameters["FROM_ATTR"] = $tok["value"];
            $lex->invalidateToken();
        } else {
            fwrite( STDERR, "Query error!\nDetails: redundant DOT" );
            exit( 80 );
        }
    }
}

function CONDITION_ELEMENT_OR_ATTRIBUTE( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::IDENTIFIER ) {
        $queryParameters["WHERE_ELEM"] = $tok["value"];
        $lex->invalidateToken();

        if ( $lex->getToken()["type"] === Scanner::DOT ) {
            $lex->invalidateToken();

            $tok = $lex->getToken();
            if ( $tok["type"] === Scanner::IDENTIFIER ) {
                $queryParameters["WHERE_ATTR"] = $tok["value"];
                $lex->invalidateToken();
            } else {
                fwrite( STDERR, "Query error!\nDetails: element identifier containing DOT character" );
                exit( 80 );
            }
        }
    } elseif ( $tok["type"] === Scanner::DOT ) {
        $lex->invalidateToken();

        $tok = $lex->getToken();
        if ( $tok["type"] === Scanner::IDENTIFIER ) {
            $queryParameters["WHERE_ATTR"] = $tok["value"];
            $lex->invalidateToken();
        } else {
            fwrite( STDERR, "Query error!\nDetails: redundant DOT" );
            exit( 80 );
        }
    }
}

function WHERE_CLAUSE( Scanner $lex )
{
    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::WHERE ) {
        $lex->invalidateToken();

        CONDITION( $lex );
    }
}

function RELATION_OPERATOR( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::EQUALS ) {
        $queryParameters["WHERE_RELATION"] = "EQUALS";
        $lex->invalidateToken();
    } elseif ( $tok["type"] === Scanner::LESS ) {
        $queryParameters["WHERE_RELATION"] = "LESS";
        $lex->invalidateToken();
    } elseif ( $tok["type"] === Scanner::GREATER ) {
        $queryParameters["WHERE_RELATION"] = "GREATER";
        $lex->invalidateToken();
    } elseif ( $tok["type"] === Scanner::CONTAINS ) {
        $queryParameters["WHERE_RELATION"] = "CONTAINS";
        $lex->invalidateToken();
    } else {
        fwrite( STDERR, "Query error!\nDetails: illegal relation operator" );
        exit( 80 );
    }
}

function CONDITION( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::NOT ) {
        $queryParameters["CONDITION"] = !$queryParameters["CONDITION"];
        $lex->invalidateToken();

        CONDITION( $lex );
    } else {
        CONDITION_ELEMENT_OR_ATTRIBUTE( $lex );
        RELATION_OPERATOR( $lex );
        LITERAL( $lex );
    }
}

function LITERAL( Scanner $lex )
{
    global $queryParameters;

    $tok = $lex->getToken();
    if ( $tok["type"] === Scanner::STRING ) {
        $queryParameters["WHERE_LITERAL"] = $tok["value"];
        $lex->invalidateToken();
    } elseif ( $tok["type"] === Scanner::NUMBER ) {
        $queryParameters["WHERE_LITERAL"] = $tok["value"];
        $lex->invalidateToken();
    } else {
        fwrite( STDERR, "Query error!\nDetails: illegal literal\n" );
        exit( 80 );
    }
}

// function ORDER_CLAUSE( Lex $lex ) {}