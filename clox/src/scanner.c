#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  const char* start; // beginning of the current lexeme
  const char* current; // character being looked at
  int line;
} Scanner;

// top-level module variable to avoid passing it around
Scanner scanner;

void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

// consume and return the current character
static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

// return the current (unconsumed) character without consuming it
static char peek() {
  return *scanner.current;
}

// return the character after the current one
static char peekNext() {
  if (isAtEnd()) return '\0';
  return scanner.current[1];
}

// look for a character and consume it, if found
static bool match(char expected) {
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

// create a token
static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

// create an error token
static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}

static void skipWhitespace() {
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        scanner.line++; // new line
        advance();
        break;
      // skip comments
      case '/':
        if (peekNext() == '/') {
          // a comment goes until the end of the line
          while (peek() != '\n' && !isAtEnd()) advance();
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static Token number() {
  while (isDigit(peek())) advance();

  // look for the decimal part
  if (peek() == '.' && isDigit(peekNext())) {
    advance(); // consume the dot
    while (isDigit(peek())) advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static Token string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') scanner.line++;
    advance();
  }

  if (isAtEnd()) return errorToken("Unterminated string.");

  advance(); // closing quote
  return makeToken(TOKEN_STRING);
}

Token scanToken() {
  skipWhitespace();
  // we are at the beginning of a new token
  scanner.start = scanner.current;

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();
  // numbers
  if (isDigit(c)) return number();

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    // characters possibly followed by =
    case '!':
      return makeToken(
        match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(
        match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(
        match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(
        match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    // strings
    case '"': return string();
  }

  return errorToken("Unexpected character.");
}
