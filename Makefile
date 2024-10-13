lint:
	cpplint --recursive --exclude=build --exclude=scrap --filter=-legal/copyright,-readability/casting,-build/include_what_you_use .
