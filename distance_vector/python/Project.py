import argparse
from NetworkSimulator import NetworkSimulator


def main():
    parser = argparse.ArgumentParser(description="route calculation simulator")
    parser.add_argument(
        "--tracelevel", "-t", type=int, default=0, help="set debug trace level"
    )
    parser.add_argument(
        "--haschange", "-c", action="store_true", help="has link change messages"
    )
    parser.add_argument("--seed", "-s", type=int, default=0, help="random seed value")
    args = parser.parse_args()

    sim = NetworkSimulator(args.haschange, args.tracelevel, args.seed)
    sim.runSimulator()


if __name__ == "__main__":
    main()
