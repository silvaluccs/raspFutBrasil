package com.lucas.oliveira.rasp_fut.proxy.api;

import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import com.fasterxml.jackson.databind.JsonNode;

@FeignClient(name = "web-futebol-client", url = "http://webws.365scores.com")
public interface WebFutebolClient {

  @RequestMapping(value = "web/games/current/?appTypeId=5&langId=31&timezoneName=America/Bahia&userCountryId=21&competitions=113", method = RequestMethod.GET)
  JsonNode getFootballMatch();

}
