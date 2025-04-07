.PHONY: all clean common dp1 dp2 dc

all: common dp1 dp2 dc

common:
	$(MAKE) -C common all

dp1: common
	$(MAKE) -C DP-1 all

dp2: common
	$(MAKE) -C DP-2 all

dc: common
	$(MAKE) -C DC all

clean:
	$(MAKE) -C common clean
	$(MAKE) -C DP-1 clean
	$(MAKE) -C DP-2 clean
	$(MAKE) -C DC clean