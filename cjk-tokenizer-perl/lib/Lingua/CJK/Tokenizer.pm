package Lingua::CJK::Tokenizer;

use strict;
use XSLoader;

XSLoader::load 'Lingua::CJK::Tokenizer';

1;
__END__

=pod

=head1 NAME

Lingua::CJK::Tokenizer - CJK Tokenizer

=head1 SYNOPSIS

    my $tknzr = Lingua::CJK::Tokenizer->new();
    $tknzr->ngram_size(5);
    $tknzr->max_token_count(100);
    $tokens_ref = $tknzr->tokenize("CJK Text");
    $tokens_ref = $tknzr->segment("CJK Text");
    $tokens_ref = $tknzr->split("CJK Text");
    $flag = $tknzr->has_cjk("CJK Text");
    $flag = $tknzr->has_cjk_only("CJK Text");

=head1 COPYRIGHT

Copyright (c) 2007 Yung-chung Lin. 

This program is free software; you can redistribute it and/or modify
it under the MIT License.

=cut
