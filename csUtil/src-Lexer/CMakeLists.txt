list(APPEND csLexer_HEADERS
  include/cs/Lexer/AbstractParser.h
  include/cs/Lexer/ConvWrapperImpl.h
  include/cs/Lexer/Lexer.h
  include/cs/Lexer/LexerConfig.h
  include/cs/Lexer/ParseException.h
  include/cs/Lexer/Scanner.h
  include/cs/Lexer/Scanners.h
  include/cs/Lexer/Scanners/CharLiteral.h
  include/cs/Lexer/Scanners/CharSequence.h
  include/cs/Lexer/Scanners/CIdentifier.h
  include/cs/Lexer/Scanners/CIntegral.h
  include/cs/Lexer/Scanners/CReal.h
  include/cs/Lexer/Scanners/CString.h
  include/cs/Lexer/Token.h
  include/cs/Lexer/TokenUtil.h
  include/cs/Lexer/ValueToken.h
)

list(APPEND csLexer_SOURCES
  src/AbstractParser.cpp
  src/Lexer.cpp
  src/ParseException.cpp
  src/Scanner.cpp
  src/Scanners/CharLiteral.cpp
  src/Scanners/CharSequence.cpp
  src/Scanners/CIdentifier.cpp
  src/Scanners/CString.cpp
  src/Token.cpp
  src/main_lexer.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csLexer_HEADERS}
  PRIVATE ${csLexer_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
