lint:
	cpplint --recursive --exclude=build --exclude=scrap --filter=-legal/copyright,-readability/casting .
