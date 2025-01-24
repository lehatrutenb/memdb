.DEFAULT_GOAL := all

all: CreateBuildLib BuildRunCmakeDebug

CreateBuildLib:
	-mkdir memdb_build
	cd memdb_build && cmake ..

ClearBuild:
	rm -rf memdb_build

BuildCmakeRelease:
	cd memdb_build && cmake .. -DSetDebugBuildType=OFF -DEnableCoverage=OFF
	cd memdb_build && cmake --build .

BuildRunCmakeDebug:
	cd memdb_build && cmake .. -DSetDebugBuildType=ON -DEnableCoverage=OFF
	cd memdb_build && cmake --build .

	cd memdb_build && ./memdb/internal/dataBase/Memdb_dbTest
