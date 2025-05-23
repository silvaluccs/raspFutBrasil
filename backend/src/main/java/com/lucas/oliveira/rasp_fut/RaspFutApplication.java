package com.lucas.oliveira.rasp_fut;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.openfeign.EnableFeignClients;
import org.springframework.context.annotation.EnableAspectJAutoProxy;

@SpringBootApplication
@EnableAspectJAutoProxy
@EnableFeignClients
public class RaspFutApplication {

  public static void main(String[] args) {
    SpringApplication.run(RaspFutApplication.class, args);
  }

}
