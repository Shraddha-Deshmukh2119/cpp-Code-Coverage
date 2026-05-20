package com.ecoomerce.sportscenter.service;

import com.ecoomerce.sportscenter.entity.Basket;
import com.ecoomerce.sportscenter.entity.BasketItem;
import com.ecoomerce.sportscenter.model.BasketResponse;
import com.ecoomerce.sportscenter.repository.BasketRepository;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.List;
import java.util.Optional;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class BasketServiceImplTest {

    @Mock
    private BasketRepository basketRepository;

    @InjectMocks
    private BasketServiceImpl basketService;

    @Test
    void getAllBaskets_returnsMappedList() {
        Basket basket = sampleBasket("b1");
        when(basketRepository.findAll()).thenReturn(List.of(basket));

        List<BasketResponse> result = basketService.getAllBaskets();

        assertThat(result).hasSize(1);
        assertThat(result.get(0).getId()).isEqualTo("b1");
        assertThat(result.get(0).getItems()).hasSize(1);
    }

    @Test
    void getBasketById_returnsResponseWhenPresent() {
        Basket basket = sampleBasket("b1");
        when(basketRepository.findById("b1")).thenReturn(Optional.of(basket));

        BasketResponse result = basketService.getBasketById("b1");

        assertThat(result).isNotNull();
        assertThat(result.getId()).isEqualTo("b1");
    }

    @Test
    void getBasketById_returnsNullWhenMissing() {
        when(basketRepository.findById("missing")).thenReturn(Optional.empty());

        BasketResponse result = basketService.getBasketById("missing");

        assertThat(result).isNull();
    }

    @Test
    void deleteBasketById_delegatesToRepository() {
        basketService.deleteBasketById("b1");

        verify(basketRepository).deleteById("b1");
    }

    @Test
    void createBasket_savesAndReturnsResponse() {
        Basket basket = sampleBasket("b2");
        when(basketRepository.save(basket)).thenReturn(basket);

        BasketResponse result = basketService.createBasket(basket);

        assertThat(result.getId()).isEqualTo("b2");
        verify(basketRepository).save(basket);
    }

    private Basket sampleBasket(String id) {
        BasketItem item = new BasketItem();
        item.setId(1);
        item.setName("Ball");
        item.setDescription("desc");
        item.setPrice(500L);
        item.setPictureUrl("url");
        item.setProductBrand("Nike");
        item.setProductType("Shoes");
        item.setQuantity(2);

        Basket basket = new Basket(id);
        basket.setItems(List.of(item));
        return basket;
    }
}
