list(APPEND csLexer_HEADERS
  include/cs/Lexer/Context.h
  include/cs/Lexer/ConvWrapperImpl.h
  include/cs/Lexer/Lexer.h
  include/cs/Lexer/Scanner.h
  include/cs/Lexer/Scanners/CharLiteral.h
  include/cs/Lexer/Scanners/CIdentifier.h
  include/cs/Lexer/Scanners/CIntegral.h
  include/cs/Lexer/Scanners/CReal.h
  include/cs/Lexer/Scanners/CString.h
  include/cs/Lexer/Token.h
  include/cs/Lexer/TokenUtil.h
  include/cs/Lexer/ValueToken.h
)

list(APPEND csLexer_SOURCES
  src/main_lexer.cpp
  src/Token.cpp
)

### csUtil ###################################################################

target_sources(csUtil
  PRIVATE ${csLexer_HEADERS}
  PRIVATE ${csLexer_SOURCES}
)

target_include_directories(csUtil
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
