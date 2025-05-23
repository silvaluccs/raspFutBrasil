package com.lucas.oliveira.rasp_fut.proxy.api;

import org.springframework.cloud.openfeign.FeignClient;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.fasterxml.jackson.databind.JsonNode;

@FeignClient(name = "web-futebol-client", url = "http://webws.365scores.com")
public interface WebFutebolClient {

  @GetMapping("web/games/current/?appTypeId=5&langId=31&timezoneName=America/Bahia&userCountryId=21&competitions=113")
  JsonNode getFootballMatchs();

  @GetMapping("web/game/stats/?langId=31")
  JsonNode getFootballMatch(@RequestParam("games") Long gameId);

}
