/*
 Copyright (C) 2010-2013 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__Parser__
#define __TrenchBroom__Parser__

#include "Exceptions.h"
#include "StringUtils.h"
#include "IO/Token.h"

namespace TrenchBroom {
    namespace IO {
        template <typename TokenType>
        class Parser {
        private:
            typedef TokenTemplate<TokenType> Token;
        protected:
            void expect(const TokenType typeMask, const Token& token) const {
                if ((token.tokenType() & typeMask) == 0) {
                    ParserException e;
                    e << "Expected " << tokenName(typeMask) << ", but got " << tokenName(token.type()) << " at " << token.line() << "," << token.column();
                    throw e;
                }
            }
        private:
            virtual String tokenName(const TokenType typeMask) const = 0;
        };
    }
}

#endif /* defined(__TrenchBroom__Parser__) */
