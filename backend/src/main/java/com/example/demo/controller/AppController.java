package com.example.demo.controller;

import org.springframework.web.bind.annotation.RestController;
import com.example.demo.service.AppService;
import org.springframework.web.bind.annotation.GetMapping;

@RestController
public class AppController {

  private final AppService appService;

  public AppController(AppService appService) {
    this.appService = appService;
  }

  @GetMapping("/partidas")
  public String partidas() {

    try {
      return appService.getPartidas().toString();
    } catch (Exception e) {
      return e.getMessage();
    }

  }

}
