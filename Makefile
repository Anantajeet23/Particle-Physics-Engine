# Compiler and flags
CXX = g++
CXXFLAGS = -I/opt/homebrew/include -Wno-deprecated-declarations -c

# Linker flags
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lglut -framework OpenGL

# Source and header directories
SRC_DIR = lancelot
HEADERS = $(SRC_DIR)/precision.h $(SRC_DIR)/core.h $(SRC_DIR)/particle.h $(SRC_DIR)/pfgen.h $(SRC_DIR)/pcontacts.h $(SRC_DIR)/world.h
SOURCES = $(SRC_DIR)/core.cpp $(SRC_DIR)/particle.cpp $(SRC_DIR)/pfgen.cpp $(SRC_DIR)/pcontacts.cpp $(SRC_DIR)/world.cpp myfireworks.cpp #change this to check out spin
OBJECTS = $(SOURCES:.cpp=.o)

# Output binary
OUTPUT = myfireworks

# Default target
all: $(OUTPUT)

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -o $@

# Link object files into the final executable
$(OUTPUT): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(OUTPUT) $(LDFLAGS)

# Clean up object files and binary
clean:
	rm -f $(OBJECTS) $(OUTPUT)