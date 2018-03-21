<?php

/**
 * Created by PhpStorm.
 * User: Marty
 * Date: 15-Mar-17
 * Time: 16:48
 */
class Scanner
{
    private $query = "";
    private $index = 0;
    private $tokenValid = FALSE;

    private $token = array(
        "type" => self::UNKNOWN,
        "value" => "",
    );

    const UNKNOWN = 0;
    const DOT = 1;
    const EQUALS = 2;
    const GREATER = 3;
    const LESS = 4;
    const IDENTIFIER = 5;
    const NUMBER = 6;
    const STRING = 7;
    const SELECT = 8; // keywords
    const FROM = 9;
    const ROOT = 10;
    const WHERE = 11;
    const NOT = 12;
    const CONTAINS = 13;
    const LIMIT = 14;
    const EOF = 15;

    /**
     * Lex constructor.
     *
     * @param $query
     */
    public function __construct( $query )
    {
        $this->query = $query;
    }

    /**
     * @return array
     */
    public function getToken()
    {
        if ( $this->tokenValid ) {
            return $this->token;
        } else {
            $this->tokenValid = TRUE;

            if ( $this->index == strlen( $this->query ) ) {
                $this->token["type"] = self::EOF;
                $this->token["value"] = "";

                return $this->token;
            }

            while ( TRUE ) {
                switch ( $this->query[$this->index] ) {
                    case " ":
                    case "\n":
                    case "\t":
                        $this->index++;
                        break;
                    case ".":
                        $this->token["type"] = self::DOT;
                        $this->token["value"] = "";
                        $this->index++;

                        return $this->token;
                    case "<":
                        $this->token["type"] = self::LESS;
                        $this->token["value"] = "";
                        $this->index++;

                        return $this->token;
                    case ">":
                        $this->token["type"] = self::GREATER;
                        $this->token["value"] = "";
                        $this->index++;

                        return $this->token;
                    case "=":
                        $this->token["type"] = self::EQUALS;
                        $this->token["value"] = "";
                        $this->index++;

                        return $this->token;
                    case "\"":
                        // string
                        $returnString = "";
                        $this->index++;
                        while ( $this->query[$this->index] !== "\"" ) {
                            $returnString .= $this->query[$this->index];
                            $this->index++;

                            if ( $this->index == strlen( $this->query ) ) {
                                fwrite(STDERR, "Query error!\nDetails: string not terminated\n");
                                exit(80);
                            }
                        }
                        $this->token["type"] = self::STRING;
                        $this->token["value"] = $returnString;
                        $this->index++;

                        return $this->token;
                    case (preg_match( "/^[a-zA-Z]$/", $this->query[$this->index] ) ? TRUE : FALSE):
                        // identifier or keyword
                        $returnString = "";
                        do {
                            $returnString .= $this->query[$this->index];
                            $this->index++;

                            if ( $this->index == strlen( $this->query ) ) {
                                break;
                            }
                        } while ( preg_match( "/^[a-zA-Z0-9]$/", $this->query[$this->index] ) ? TRUE : FALSE );

                        $this->token["value"] = "";

                        switch ( $returnString ) {
                            case "SELECT":
                                $this->token["type"] = self::SELECT;
                                break;
                            case "FROM":
                                $this->token["type"] = self::FROM;
                                break;
                            case "ROOT":
                                $this->token["type"] = self::ROOT;
                                break;
                            case "WHERE":
                                $this->token["type"] = self::WHERE;
                                break;
                            case "NOT":
                                $this->token["type"] = self::NOT;
                                break;
                            case "CONTAINS":
                                $this->token["type"] = self::CONTAINS;
                                break;
                            case "LIMIT":
                                $this->token["type"] = self::LIMIT;
                                break;
                            default:
                                $this->token["type"] = self::IDENTIFIER;
                                $this->token["value"] = $returnString;
                        }

                        return $this->token;
                    case (preg_match( "/^[\\-0-9]$/", $this->query[$this->index] ) ? TRUE : FALSE):
                        // number
                        $returnString = "";
                        do {
                            $returnString .= $this->query[$this->index];
                            $this->index++;

                            if ( $this->index == strlen( $this->query ) ) {
                                break;
                            }
                        } while ( preg_match( "/^[0-9]$/", $this->query[$this->index] ) ? TRUE : FALSE );

                        $this->token["type"] = self::NUMBER;
                        $this->token["value"] = $returnString;

                        return $this->token;
                    default:
                        $this->token["type"] = self::UNKNOWN;
                        $this->token["value"] = "";

                        return $this->token;
                }
            }
        }

        return $this->token;
    }

    public function invalidateToken()
    {
        $this->tokenValid = FALSE;
    }

    /**
     * @return int
     */
    public function getIndex()
    {
        return $this->index;
    }

    /**
     * @return string
     */
    public function getQuery()
    {
        return $this->query;
    }
}