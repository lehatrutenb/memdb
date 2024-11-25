CreateBuildLib:
	-mkdir memdb_build
	cd memdb_build && cmake ..

BuildCmakeRelease:
	cd memdb_build && cmake .. -DSetDebugBuildType=OFF -DEnableCoverage=OFF
	cd memdb_build && cmake --build .

BuildRunCmakeDebug:
	cd memdb_build && cmake .. -DSetDebugBuildType=ON -DEnableCoverage=OFF
	cd memdb_build && cmake --build .

	cd memdb_build && ./memdb/internal/dataBase/Memdb_dbTest
  #cd memdb_build && ./Stack/StackTest && ./Parser/ParserTest && ./Preprocessor/PreprocessorTest && ./Emulator/EmulatorTest && ./Runner ./Stack/StackTest
#./internal/EmulatorTest

#BuildRunCmakeCoverage:
  #cd Emulator_build && lcov --directory . --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1

  #cd Emulator_build && cmake ../Emulator -DSetDebugBuildType=ON -DEnableCoverage=ON
  #cd Emulator_build && cmake --build .

  #cd Emulator_build && ./Stack/StackTest && ./Emulator/EmulatorTest && ./Preprocessor/PreprocessorTest && ./Parser/ParserTest

  #cd Emulator_build && genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report/