import { WorldCupResultGateway } from "./infra/gateways/world-cup-results.gateway";

const worldCupResultsGateway = new WorldCupResultGateway();

async function main() {
  const matches = await worldCupResultsGateway.getAllMatchToday();
  console.log(matches);

  const match = await worldCupResultsGateway.getMatchFromId(4697923);
  console.log(match);
}

main();
